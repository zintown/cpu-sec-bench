#include <stdio.h>
#include <Windows.h>
#include <imagehlp.h>
#include <locale.h>
#include <iostream>
#pragma comment(lib,"DbgHelp.lib")
 
BOOL CALLBACK CallBackProc( PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext )
{
    //printf( "函数名: %s \r\n地址: %08X \r\n\r\n", pSymInfo->Name, pSymInfo->Address );
    std::cerr << pSymInfo->Name << "\n" << pSymInfo->Address << std::endl;
    return TRUE;
}
 
BOOL GetSymbol( const char* FileName )
{
    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId() );
    //CloseHandle( hProcess );
    
    if ( !SymInitialize( hProcess, NULL, FALSE ) )
    {
        std::cerr << "SymInitialize failed ";
        return FALSE;
    }
    
    DWORD dwOpt = SymGetOptions();
    SymSetOptions( dwOpt | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_CASE_INSENSITIVE );
    
    DWORD64 dwSymModule = SymLoadModuleEx( hProcess, NULL, FileName, NULL, 0, 0, NULL, 0 );
    
    if ( 0 == dwSymModule )
    {
        std::cerr << "dwSymModule failed " << GetLastError();
        SymCleanup( hProcess );
        return FALSE;
    }
    
    IMAGEHLP_LINE64 line = { 0 };
    line.SizeOfStruct = sizeof(line);
    PDWORD   lpDisplacement;

    CONTEXT context;
    GetThreadContext(hProcess,&context);

    if ( !SymEnumSymbols( hProcess, dwSymModule, 0, ( PSYM_ENUMERATESYMBOLS_CALLBACK )CallBackProc, NULL ) )
    //if ( !SymGetLineFromAddr64( hProcess, (DWORD64)&GetSymbol, lpDisplacement, &line ) )
    {
        std::cerr << "SymEnumSymbols failed " << GetLastError();
        SymCleanup( hProcess );
        return FALSE;
    }
    
    return SymCleanup( hProcess );
}
 
int main( int argc, char* argv[] )
{
    //const TCHAR* sDllPath = _T( "C:\\Users\\zintown\\cpu-sec-bench\\4.EXE" );
    
    const char* sDllPath =  "C:\\Users\\zintown\\cpu-sec-bench\\6.exe";
    if ( !GetSymbol( sDllPath ) )
    {
        return -1;
    }
    int b = 0;
    getchar();
    return 0;
}