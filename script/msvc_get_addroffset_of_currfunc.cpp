#include <windows.h>
#include <dbghelp.h>
#include <WinError.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]){
    LONG   lDisplacement;
    PCSTR test_case_filename = (PCSTR)argv[1];
    std::string str_line = std::string(argv[2]);
    DWORD  dwLineNumber = (DWORD)std::stoul(str_line);

    IMAGEHLP_LINE64 line;
    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    SymSetOptions(SYMOPT_LOAD_LINES);
    bool could_initialize_sym = SymInitialize(hProcess, NULL, TRUE);
    if(!could_initialize_sym) {
        #ifdef DEBUG_OUTPUT
            std::cerr << "error to initialize symbols: " << std::endl;
        #endif
        return 0;
    }

    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    if (SymGetLineFromName64(hProcess, test_case_filename, NULL,
        dwLineNumber, &lDisplacement, &line))
    {
        std::cout << "line address is: " << std::hex << line.Address << "line number is: " << std::dec << line.LineNumber << std::endl;
    }
    else
    {
        // SymGetLineFromName64 failed
        DWORD error = GetLastError();
        #ifdef DEBUG_OUTPUT
            std::cerr << "SymGetLineFromName64 returned error: " << error << std::endl;
        #endif
    }
}