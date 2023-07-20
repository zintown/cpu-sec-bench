#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <iostream>
#include <stdio.h>

int main(){
    DWORD  dwLineNumber;
    LONG   lDisplacement;
    // TCHAR  szModuleName[MAX_PATH];
    // TCHAR  szFileName[MAX_PATH];
    IMAGEHLP_LINE64 line;
    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId() );
    SymSetOptions(SYMOPT_LOAD_LINES);
    bool could_initialize_sym = SymInitialize(hProcess, NULL, TRUE);
    if(!could_initialize_sym) {
        std::cout << "error to initialize symbols: " << std::endl;
        return 0;
    }

    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    // _tcscpy_s(szModuleName, MAX_PATH, TEXT("4"));
    // _tcscpy_s(szFileName, MAX_PATH, TEXT("4.cpp"));
GOTO_LABEL:
    // if (SymGetLineFromName64(hProcess, "8", "8.cpp",
    //     10, &lDisplacement, &line))
    // {
    //     std::cout << "line address is: " << std::hex << line.Address << "line number is: " << std::dec << line.LineNumber << std::endl;
    // }
    // else
    // {
    //     // SymGetLineFromName64 failed
    //     DWORD error = GetLastError();
    //     _tprintf(TEXT("SymGetLineFromName64 returned error : %d\n"), error);
    // }
    // if (SymGetLineFromName64(hProcess, NULL, NULL,
    //     25, &lDisplacement, &line))
    // {
    //     std::cout << "line address is: " << std::hex << line.Address << "line number is: " << std::dec << line.LineNumber << std::endl;
    // }
    // else
    // {
    //     // SymGetLineFromName64 failed
    //     DWORD error = GetLastError();
    //     _tprintf(TEXT("SymGetLineFromName64 returned error : %d\n"), error);
    // }
    std::cout << __FILE__ << std::endl;
}