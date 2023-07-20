#include<iostream>
#include<Windows.h>
#include<DbgHelp.h>
#include <comdef.h>
#include <WinError.h>

// int b = 1;

// #define ASSIGN(val) \
//     int abc = val

// BOOL CALLBACK run(PSRCCODEINFO code_info, PVOID UserContext){
//             std::cout << "current line number is: " << code_info->LineNumber 
//                       << "current address is:     "<<  code_info->Address << std::endl;
//         return true;
// }

// std::string get_last_error_message(DWORD errorMessageID) {
//     //Get the error message ID, if any.
//     if(errorMessageID == 0) {
//         return std::string(); //No error message has been recorded
//     }
    
//     LPSTR messageBuffer = nullptr;

//     //Ask Win32 to give us the string version of that message ID.
//     //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
//     size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
//                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
//     //Copy the error message into a std::string.
//     std::string message(messageBuffer, size);
    
//     //Free the Win32's string's buffer.
//     LocalFree(messageBuffer);
            
//     return message;
// }

int main(){
    // int a = 0;

    // PIMAGEHLP_SYMBOL  mysymbol;
    // HANDLE         process;
    // PDWORD dwDisplacement;
    // //std::cout << "my symbol is: " << mysymbol->Address << std::endl;
    // process = GetCurrentProcess();
    // bool could_initialize_sym = SymInitialize(process, NULL, TRUE);
    // if(!could_initialize_sym) {
    //     std::cout << "error to initialize symbols: " << std::endl;
    //     return 0;
    // }
    // mysymbol = (PIMAGEHLP_SYMBOL)calloc(sizeof(IMAGEHLP_SYMBOL) + 256 * sizeof(char), 1);
    // mysymbol->MaxNameLength = 255;
    // mysymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
    // SymGetSymFromName64 (process, "main", mysymbol);
    // std::cout << std::hex << "my symbol is: " << mysymbol->Address << std::endl;

    // IMAGEHLP_LINE64  myline;
    // myline.SizeOfStruct = sizeof(myline);
    // bool sl = SymGetLineFromAddr64(process, mysymbol->Address,dwDisplacement, (PIMAGEHLP_LINE64)&myline); // Retruns true on success
    // if(!sl) std::cout << "symgetline failed" << std::endl;
    // else std::cout << std::hex << "main line is: " << myline.LineNumber << std::endl;
    // struct SymEnumSourceFilesContext {
    //     HANDLE process;
    //     DWORD64 start_address;
    // } context;

    // DWORD64  base_dll = SymLoadModuleEx(process, NULL, NULL, NULL, 0,0,0,0);
    // SymEnumSourceFiles(process,NULL,NULL, [](PSOURCEFILE source_file, PVOID user_context) -> BOOL {
    //     SymEnumSourceFilesContext* context = (SymEnumSourceFilesContext*)user_context;
    //         SymEnumLines(context->process, context->start_address,NULL,NULL,[](PSRCCODEINFO line, PVOID user_context) -> BOOL {
    //             std::cerr << "current line number is: " << std::endl;
    //             return true;
    //         },NULL);
    //     return true;
    // }, NULL);
    // const char* lpszFile = "4.cpp";
    //  DWORD64 BaseOfDll = SymLoadModuleEx(process,
    //                                     NULL,
    //                                    lpszFile,
    //                                     NULL,0,0,NULL,0);
    // BOOL sf = SymEnumSourceFilesW(process,BaseOfDll,NULL, [](PSOURCEFILEW source_file, PVOID user_context) -> BOOL {
    //     SymEnumSourceFilesContext* context = (SymEnumSourceFilesContext*)user_context;
    //         BOOL sl = SymEnumLines(context->process, context->start_address,NULL,NULL,[](PSRCCODEINFO line, PVOID user_context) -> BOOL {
    //             std::cerr << "current line number is: " << std::endl;
    //             return true;
    //         },NULL);
    //         if(!sl) std::cerr << "SymEnumLines Failed" << std::endl;
    //     return true;
    // }, NULL);

    // if(!sf) {
    //     long code = GetLastError();
    //     std::cerr << "SymEnumSourceFiles Failed: " << code << std::endl;
    //     get_last_error_message(code);
    // }
//     ASSIGN(2);
// GOTO_LABEL:

//     a++;

IMAGEHLP_SYMBOL64 *pSymbol;
PDWORD   lpDisplacement;
HANDLE process = OpenProcess( PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId() );

bool could_initialize_sym = SymInitialize(process, NULL, TRUE);
if(!could_initialize_sym) {
    std::cout << "error to initialize symbols: " << std::endl;
    return 0;
}

// pSymbol = (IMAGEHLP_SYMBOL64*)new BYTE[sizeof(IMAGEHLP_SYMBOL64)+MAX_SYM_NAME];
// memset(pSymbol, 0, sizeof(IMAGEHLP_SYMBOL64) + MAX_SYM_NAME);
// pSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64); // Required
// pSymbol->MaxNameLength = MAX_SYM_NAME;             // Required
// SymGetSymFromName64(process, "main", pSymbol); // Retruns true on success
// std::cerr << std::hex << pSymbol->Address << std::endl;

IMAGEHLP_LINE64 line = { 0 };
line.SizeOfStruct = sizeof(line) + 1024;

CONTEXT context;
context.ContextFlags = CONTEXT_FULL;
HANDLE thread = GetCurrentThread();
GetThreadContext(thread,&context);

std::cout << std::hex << "context 's rip is: " << context.Rip << std::endl;

// BOOL bSuccess = SymGetLineFromAddr64(process, (DWORD64)&main,
//                                lpDisplacement, &line);

// if(bSuccess)
// {
//    std::cerr << "current line number is: " << line.LineNumber << std::endl;
// }else{
//     std::cerr << "failed " << GetLastError() << std::endl;
// }
}