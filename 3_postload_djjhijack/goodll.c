#ifndef _MYDLL_H_
#define _MYDLL_H_

#define EXPORT_DLL _declspec(dllexport) 

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_DLL int add(int a, int b); // 即 int add(int a,int b)

#ifdef __cplusplus
}
#endif

#endif

int add(int a, int b) {
    return a + b;
}
