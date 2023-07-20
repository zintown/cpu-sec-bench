#include <windows.h>
#include <DbgHelp.h>
#include <Psapi.h>

#include <iostream>
#include <string>
#include <filesystem>

std::string get_last_error_message() {
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }
    
    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);
    
    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);
            
    return message;
}

DWORD64 load_module(HANDLE process, std::string_view path, DWORD64 base_addr)
{
    return SymLoadModule64(process, NULL, path.data(), NULL, base_addr, 0);
}

bool load_module_info(DWORD64 __module, PROCESS_INFORMATION &process_info)
{
    IMAGEHLP_MODULE64 module_info;
    module_info.SizeOfStruct = sizeof(module_info);
    BOOL bSuccess = SymGetModuleInfo64(process_info.hProcess, __module, &module_info);

    struct SymEnumSourceFilesContext {
        HANDLE process;
        DWORD64 start_address;
    } context;

    context.process = process_info.hProcess;
    context.start_address = __module;

    if(bSuccess && module_info.SymType == SymPdb)
    {
        SymEnumSourceFiles(context.process, context.start_address, NULL, [](PSOURCEFILE source_file, PVOID user_context) -> BOOL {
            //----------------------------------------------------^
            // Can filter files, like "*.cpp"
            // Todo: let virtualize the mask

            SymEnumSourceFilesContext* context = (SymEnumSourceFilesContext*)user_context;

            SymEnumLines(context->process, context->start_address, NULL, source_file->FileName, [](PSRCCODEINFO line, PVOID user_context) -> BOOL {

                SymEnumSourceFilesContext* context = (SymEnumSourceFilesContext*)user_context;

                std::string filename = line->FileName;
                
                if(filename.ends_with("debugee.cpp")) {
                    //show only files for debugee

                    printf("%s:%i: %p\n", filename.c_str(), (int)line->LineNumber, (void*)line->Address);
                }
                return true;
            }, (void*)context);

            return true;
        }, &context);

        return true;
    }

    return false;
}

std::string GetFileNameFromHandle(HANDLE hFile) 
{
    BOOL bSuccess = FALSE;
    TCHAR buffer[MAX_PATH+1];

    // Get the file size.
    DWORD dwFileSizeHi = 0;
    DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi); 

    if( dwFileSizeLo != 0 || dwFileSizeHi != 0 )
    {     
        // Create a file mapping object.
        HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0,  1, NULL);

        if (hFileMap) 
        {
            // Create a file mapping to get the file name.
            void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

            if (pMem) 
            {
                // Gets the full path of the file, instead of the drive letter (ie C://), it has the device name
                if (GetMappedFileName (GetCurrentProcess(), pMem,  buffer, MAX_PATH)) 
                {
                    TCHAR drive_letters[MAX_PATH+1];
                    const char* it = drive_letters;
                    if (GetLogicalDriveStrings(MAX_PATH, drive_letters)) 
                    {
                        std::string drive_path = "x:";
                            char drive_letter = *it;

                        std::string file_name = buffer;
                        while(drive_letter) {
                            drive_path[0] = drive_letter;

                            if (QueryDosDevice(drive_path.data(), buffer, MAX_PATH))
                            {
                                std::string_view view = buffer;
                                if(file_name.starts_with(view)) {
                                    return drive_path + file_name.substr(view.size());
                                }
                            } else {
                                printf("failed to query dos device for drive letter '%c': %s\n", drive_letter, get_last_error_message().c_str());
                                break;
                            }

                            //removes the drive name and the null termination
                            while (*it++);
                        }
                    } else {
                        printf("failed to get logical drives: '%s'\n", get_last_error_message().c_str());
                    }
                }
                else {
                    printf("failed to get get mapped file names: '%s'\n", get_last_error_message().c_str());
                }
            }
        }
    }

    return "unknow path";
}

int main()
{
    PROCESS_INFORMATION m_process_info;
    STARTUPINFO m_startup_info;
    DEBUG_EVENT m_debug_event = { 0 };

    ZeroMemory( &m_startup_info, sizeof(m_startup_info) ); 
    m_startup_info.cb = sizeof(m_startup_info); 

    ZeroMemory( &m_process_info, sizeof(m_process_info) );

    std::string debugee_path = std::filesystem::absolute("debugee.exe").string();

    if(!CreateProcess (debugee_path.c_str(), NULL, NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS, NULL,NULL, &m_startup_info, &m_process_info )) {
        std::cout << "error to create process: " << get_last_error_message() << std::endl;
        return 0;
    }

    BOOL could_initialize_sym = SymInitialize(m_process_info.hProcess, NULL, false);

    if(!could_initialize_sym) {
        std::cout << "error to initialize symbols: " << get_last_error_message() << std::endl;
        return 0;
    }

    while(1) {
        WaitForDebugEvent(&m_debug_event, INFINITE); 

        DWORD status = DBG_CONTINUE;

        switch (m_debug_event.dwDebugEventCode) 
        { 
            case CREATE_PROCESS_DEBUG_EVENT:
            {
                std::string process_name = GetFileNameFromHandle(m_debug_event.u.CreateProcessInfo.hFile);
                DWORD64 __module = load_module(m_process_info.hProcess, process_name, (DWORD64)m_debug_event.u.CreateProcessInfo.lpStartAddress);

                if(__module) {
                    bool modules_has_been_loaded = load_module_info(__module, m_process_info);
                }
            
                break;
            }
            default:
            break;
        }

        ContinueDebugEvent(m_debug_event.dwProcessId, m_debug_event.dwThreadId, status);
    }
}