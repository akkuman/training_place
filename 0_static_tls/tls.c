#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "User32.lib")

void NTAPI tls_callback(void *DllHandle, DWORD Reason, void *Reserved)
{
    if (Reason == DLL_PROCESS_ATTACH)
    {
        MessageBox(0, L"test", L"test", MB_ICONEXCLAMATION);
        Sleep(1000);
    }
}

#ifdef _WIN64
     #pragma comment (linker, "/INCLUDE:_tls_used")  // See p. 1 below
     #pragma comment (linker, "/INCLUDE:tls_callback_func")  // See p. 3 below
#else
     #pragma comment (linker, "/INCLUDE:__tls_used")  // See p. 1 below
     #pragma comment (linker, "/INCLUDE:_tls_callback_func")  // See p. 3 below
#endif

#ifdef _WIN64
#pragma const_seg(".CRT$XLB")
EXTERN_C const
#else
#pragma data_seg(".CRT$XLB")
EXTERN_C
#endif
    PIMAGE_TLS_CALLBACK tls_callback_func = tls_callback;
#ifdef _WIN64
#pragma const_seg()
#else
#pragma data_seg()
#endif //_WIN64

int main(void)
{
    printf("hello world\n");
    (void)getchar();
    return 0;
}