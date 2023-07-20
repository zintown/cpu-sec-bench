#include <Windows.h>
#include <dbghelp.h>
#include <iostream>
#pragma comment(lib,"DbgHelp.lib")

int main() {
     HANDLE process = GetCurrentProcess();
     SymInitialize( process, NULL, TRUE );

     SymSetOptions(SYMOPT_LOAD_LINES);


     void *stack[64];
     unsigned short frames = CaptureStackBackTrace( 0, 64, stack, NULL );
     std::cout << "main is: " << std::hex << (DWORD64)&main << std::endl;

     SYMBOL_INFO *symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
     symbol->MaxNameLen = 255;
     symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

     IMAGEHLP_LINE64 *line = NULL;
     DWORD lastError = 0;
     DWORD displacement;

  
     printf("Backtrace\n");
     for(int i = 0; i < frames; i++) {
         std::cerr << std::hex << "stack is: " << stack[i];
         SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
         printf("\t %i: %s\n", frames - i - 1, symbol->Name);
         line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
         line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
         if (SymGetLineFromAddr64(process, (DWORD64)stack[i]+1, &displacement, line)) {
                printf("%s %i\n", line->FileName, line->LineNumber);
         } else {
                printf("failed\n");
         }
        lastError = GetLastError();
        printf("errcode: %i\n", lastError);
        
        free(line);
        line = NULL;
     }

     free(symbol);
}