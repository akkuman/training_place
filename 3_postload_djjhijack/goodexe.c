#include "windows.h"
#include "stdio.h"

int main(int argc, char argv[]) {
    char a[10] = {0x90, 0x90, 0x90};
    HANDLE h = LoadLibrary(TEXT("./test.dll"));
    printf("%x\n", h);
}