#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

void change(){
    perror("Successfully overflowed\n");
}

long long addr = reinterpret_cast<long long>(change);

void func(){
    long long b[1];

    b[0] = addr;
    b[1] = addr;
    b[2] = addr;
    b[3] = addr;
    b[4] = addr;
}

int main(){
    int b;
    scanf("%d",&b);
    func();
    
    return 0;
}