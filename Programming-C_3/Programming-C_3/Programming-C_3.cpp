#include <iostream>

#define arrayLength 5

int intArray[arrayLength] = { 1, 2, 3, 4, 5 };
double doubleArray[arrayLength] = { 1.1, 2.2, 3.3, 4.4, 5.5 };

int main()
{
    //intのチェック
    fprintf_s(stdout, "-----int-----\n");

    for (int i = 0; i < arrayLength; i++) {
        fprintf_s(stdout, "Address:%p   Value:%d\n", intArray + i, intArray[i]);
    }    

    //doubleのチェック
    fprintf_s(stdout, "-----double-----\n");

    for (int i = 0; i < arrayLength; i++) {
        fprintf_s(stdout, "Address:%p   Value:%f\n", doubleArray + i, doubleArray[i]);
    }
}