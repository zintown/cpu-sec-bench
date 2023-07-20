// threadcontext.cpp : Defines the entry point for the console application.
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <intrin.h>
__declspec(noinline)
void* func(){
    void* p = _AddressOfReturnAddress();
    return p;
}

int main(int argc, char* argv[])
{
    void* fp = func();
    printf("return addr's addr is: %llx", fp);
    CONTEXT new_context;
    RtlCaptureContext(&new_context);
    std::cerr << "new context's rsp is: " << std::hex << new_context.Rsp << std::endl;
    return 0;
}