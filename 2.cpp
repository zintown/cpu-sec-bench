// CalculatreSum.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

extern "C" inline int AdderAsm(int, int, int);
int AdderCpp(int, int, int);

int main() {
    int sum;
    sum = AdderCpp(4, 5, 6);
    printf("Sum from AdderCpp is: %d\n", sum);
    sum = AdderAsm(4, 5, 6);
    printf("Sum from AdderAsm is: %d\n", sum);
}

int AdderCpp(int a, int b, int c) {
    return a + b + c;
}