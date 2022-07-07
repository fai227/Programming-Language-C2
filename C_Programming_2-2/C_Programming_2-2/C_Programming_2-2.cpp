#include <iostream>

void method_B();

void method_A() {
    fprintf_s(stdout, "A-Method");
}

int main()
{
    method_A();
    method_B();
}

void method_B() {
    fprintf_s(stdout, "B-Method");
}