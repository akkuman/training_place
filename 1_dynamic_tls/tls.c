#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "User32.lib")

#ifdef _WIN64
     #pragma comment (linker, "/INCLUDE:_tls_used")  // See p. 1 below
     #pragma comment (linker, "/INCLUDE:tls_callback_func")  // See p. 3 below
#else
     #pragma comment (linker, "/INCLUDE:__tls_used")  // See p. 1 below
     #pragma comment (linker, "/INCLUDE:_tls_callback_func")  // See p. 3 below
#endif

void NTAPI tls_callback(void *DllHandle, DWORD Reason, void *Reserved);
void NTAPI tls_callback_secret(PVOID hModule, DWORD dwReason, PVOID pContext);

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


void NTAPI tls_callback(void *DllHandle, DWORD Reason, void *Reserved)
{
    if (Reason == DLL_THREAD_ATTACH)
    {
        MessageBox(0, TEXT("TLS Callback: Thread Attach Triggered"), TEXT("TLS"), 0);
    }
    if (Reason == DLL_PROCESS_ATTACH)
	{
        MessageBox(0, TEXT("TLS Callback: Process Attach Triggered"), TEXT("TLS"), 0);
		// DEBUG - Help understand how this is being stored in memory.
		printf("TLS Callback Addresses:\n\tFunction Address: %p\n\tCRT Callback Address: %p\n", tls_callback, &tls_callback_func);

		// The location of the next element in the array of TLS callbacks in memory
		PIMAGE_TLS_CALLBACK *dynamic_callback = (PIMAGE_TLS_CALLBACK*)&tls_callback_func + 1;

		// The default Page Permissions do not necessairly allow us to write to this
		// part of (our) memory. We need to set Write Permissions to the memory range
		// we'll be writing to (here only one callback, thus sizeof(dynamic_callback).
		//
		// Tip: This can be done slightly more stealthy by using the PEB to access
		// kernel32.dll and call this manually.
		DWORD	old;
		VirtualProtect(dynamic_callback, sizeof(dynamic_callback), PAGE_EXECUTE_READWRITE, &old);

		// Finally, set the callback in memory, which is next in line to be
		// executed (in our case).
		*dynamic_callback = (PIMAGE_TLS_CALLBACK)tls_callback_secret;
	}
}


void NTAPI tls_callback_secret(PVOID hModule, DWORD dwReason, PVOID pContext)
{
	if (dwReason == DLL_THREAD_ATTACH)
	{
		// This will be loaded in each DLL thread attach
		MessageBox(0, TEXT("Dynamic TLS Callback: Thread Attach Triggered"), TEXT("Dynamic_TLS_Thread"), 0);
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		MessageBox(0, TEXT("Dynamic TLS Callback: Process Attach Triggered"), TEXT("TLS_Process"), 0);
		// DEBUG - Help understand how this is being stored in memory.
		printf("TLS Callback Addresses:\n\tFunction Address: %p\n\tCRT Callback Address: %p\n", tls_callback_secret, &tls_callback_func + 1);
	}
}


int main()
{
	printf("Main():\n    Hello World\n");
	system("pause");
	return 0;
}
