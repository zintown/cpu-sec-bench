#include <iostream>
#include <Windows.h>
#include <intrin.h>

PVOID *StackBase = (PVOID*) _ReturnAddress();
PVOID *StackFrame = (PVOID*)_AddressOfReturnAddress();

void func(int a){
    a += 1;
    StackBase = (PVOID*) _ReturnAddress();
    StackFrame = (PVOID*)_AddressOfReturnAddress();
    std::cout << "func stack base" << StackBase << std::endl;
    std::cout << "func stack frame"<< *StackFrame << std::endl;
}

int main(){
    int a = 0;
    std::cout << "Global stack base" << StackBase << std::endl;
    std::cout << "Global stack frame"<< *StackFrame << std::endl;
    func(a);
}