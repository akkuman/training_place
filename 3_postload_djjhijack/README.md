# 介绍

实现了文章 [红队开发 - 白加黑自动化生成器](https://mp.weixin.qq.com/s/MZY9S2yv12b6xFcT1hlnEQ) 中提到的 Post-Load Dll 劫持 的 demo

## 尝试

main.exe 调用正常dll会打印出loadlibrary后返回的dll handle，恶意dll会调用起计算器

### 编译正常示例

```shell
xmake b good
```

### 编译恶意示例

```shell
xmake b evil
```

## 随手记

### 原理

在文章 [红队开发 - 白加黑自动化生成器](https://mp.weixin.qq.com/s/MZY9S2yv12b6xFcT1hlnEQ) 中有提到，主要就是通过劫持ntdll的LdrLoadDll堆栈的返回地址，让程序LoadLibrary之后跳到我们的程序空间。

其实和栈溢出的利用有点类似，call一个地址的时候会将该条语句的下一条语句进行压栈，以便call调用完成后能够回跳回来。

`*stack > ldrLoadDll && *stack < ldrLoadDll + 0x1000` 只是划分了一个 `LdrLoadDll` 实现的大致地址，假如如下

```
77BFDDC0 | 8BFF                     | mov edi,edi                             | LdrLoadDll
77BFDDC2 | 55                       | push ebp                                |
77BFDDC3 | 8BEC                     | mov ebp,esp                             |
77BFDDC5 | 83E4 F8                  | and esp,FFFFFFF8                        |
77BFDDC8 | 81EC 74010000            | sub esp,174                             |
77BFDDCE | A1 70B3CD77              | mov eax,dword ptr ds:[77CDB370]         |
77BFDDD3 | 33C4                     | xor eax,esp                             |
77BFDDD5 | 898424 70010000          | mov dword ptr ss:[esp+170],eax          |
77BFDDDC | 8B4D 0C                  | mov ecx,dword ptr ss:[ebp+C]            |
77BFDDDF | 8B45 14                  | mov eax,dword ptr ss:[ebp+14]           |
77BFDDE2 | 894424 10                | mov dword ptr ss:[esp+10],eax           | [esp+10]:main+33
77BFDDE6 | 53                       | push ebx                                |
77BFDDE7 | 56                       | push esi                                | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDDE8 | 8B75 10                  | mov esi,dword ptr ss:[ebp+10]           |
77BFDDEB | 57                       | push edi                                | edi:&"ALLUSERSPROFILE=C:\\ProgramData"
77BFDDEC | 8B7D 08                  | mov edi,dword ptr ss:[ebp+8]            | [ebp+8]:L"./test.dll"
77BFDDEF | 85C9                     | test ecx,ecx                            |
77BFDDF1 | 0F84 4A010000            | je ntdll.77BFDF41                       |
77BFDDF7 | 8B09                     | mov ecx,dword ptr ds:[ecx]              |
77BFDDF9 | E8 020B0000              | call ntdll.77BFE900                     |
77BFDDFE | 8BD8                     | mov ebx,eax                             |
77BFDE00 | F605 C037CD77 09         | test byte ptr ds:[77CD37C0],9           |
77BFDE07 | 0F85 91B70400            | jne ntdll.77C4959E                      |
77BFDE0D | 8B0D 1C39CD77            | mov ecx,dword ptr ds:[77CD391C]         |
77BFDE13 | F6C1 04                  | test cl,4                               |
77BFDE16 | 0F84 A6B70400            | je ntdll.77C495C2                       |
77BFDE1C | F6C3 08                  | test bl,8                               |
77BFDE1F | 0F85 B8B70400            | jne ntdll.77C495DD                      |
77BFDE25 | 64:A1 18000000           | mov eax,dword ptr fs:[18]               |
77BFDE2B | B9 00200000              | mov ecx,2000                            |
77BFDE30 | 66:8588 CA0F0000         | test word ptr ds:[eax+FCA],cx           |
77BFDE37 | 0F85 E4B70400            | jne ntdll.77C49621                      |
77BFDE3D | 8B4E 04                  | mov ecx,dword ptr ds:[esi+4]            |
77BFDE40 | 8D8424 28010000          | lea eax,dword ptr ss:[esp+128]          |
77BFDE47 | 50                       | push eax                                |
77BFDE48 | 8BD7                     | mov edx,edi                             | edi:&"ALLUSERSPROFILE=C:\\ProgramData"
77BFDE4A | E8 770A0000              | call ntdll.77BFE8C6                     |
77BFDE4F | 68 A8140000              | push 14A8                               |
77BFDE54 | 8BD6                     | mov edx,esi                             | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDE56 | 895C24 14                | mov dword ptr ss:[esp+14],ebx           | [esp+14]:L"./test.dll"
77BFDE5A | 33C9                     | xor ecx,ecx                             |
77BFDE5C | E8 F5000000              | call ntdll.77BFDF56                     |
77BFDE61 | 8D4424 28                | lea eax,dword ptr ss:[esp+28]           |
77BFDE65 | C74424 20 00000001       | mov dword ptr ss:[esp+20],1000000       |
77BFDE6D | 894424 24                | mov dword ptr ss:[esp+24],eax           | [esp+24]:"MZ?"
77BFDE71 | 8D5424 20                | lea edx,dword ptr ss:[esp+20]           |
77BFDE75 | 33C0                     | xor eax,eax                             |
77BFDE77 | 8BCE                     | mov ecx,esi                             | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDE79 | 66:894424 28             | mov word ptr ss:[esp+28],ax             |
77BFDE7E | 8D4424 10                | lea eax,dword ptr ss:[esp+10]           | [esp+10]:main+33
77BFDE82 | 50                       | push eax                                |
77BFDE83 | 6A 00                    | push 0                                  |
77BFDE85 | E8 66D2FFFF              | call ntdll.77BFB0F0                     |
77BFDE8A | 894424 14                | mov dword ptr ss:[esp+14],eax           | [esp+14]:L"./test.dll"
77BFDE8E | 85C0                     | test eax,eax                            |
77BFDE90 | 78 24                    | js ntdll.77BFDEB6                       |
77BFDE92 | 8D4424 14                | lea eax,dword ptr ss:[esp+14]           | [esp+14]:L"./test.dll"
77BFDE96 | 50                       | push eax                                |
77BFDE97 | 8D4424 1C                | lea eax,dword ptr ss:[esp+1C]           |
77BFDE9B | 50                       | push eax                                |
77BFDE9C | 6A 00                    | push 0                                  |
77BFDE9E | 6A 00                    | push 0                                  |
77BFDEA0 | 6A 04                    | push 4                                  |
77BFDEA2 | FF7424 24                | push dword ptr ss:[esp+24]              | [esp+24]:"MZ?"
77BFDEA6 | 8D9424 40010000          | lea edx,dword ptr ss:[esp+140]          |
77BFDEAD | 8D4C24 38                | lea ecx,dword ptr ss:[esp+38]           | [esp+38]:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDEB1 | E8 9E020000              | call ntdll.77BFE154                     |
77BFDEB6 | 8D4424 28                | lea eax,dword ptr ss:[esp+28]           |
77BFDEBA | 3B4424 24                | cmp eax,dword ptr ss:[esp+24]           | [esp+24]:"MZ?"
77BFDEBE | 0F85 84000000            | jne ntdll.77BFDF48                      |
77BFDEC4 | 8D4424 28                | lea eax,dword ptr ss:[esp+28]           |
77BFDEC8 | C74424 20 00000001       | mov dword ptr ss:[esp+20],1000000       |
77BFDED0 | 894424 24                | mov dword ptr ss:[esp+24],eax           | [esp+24]:"MZ?"
77BFDED4 | 8BD6                     | mov edx,esi                             | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDED6 | 33C0                     | xor eax,eax                             |
77BFDED8 | 33C9                     | xor ecx,ecx                             |
77BFDEDA | 68 A9140000              | push 14A9                               |
77BFDEDF | 66:894424 2C             | mov word ptr ss:[esp+2C],ax             |
77BFDEE4 | E8 6D000000              | call ntdll.77BFDF56                     |
77BFDEE9 | 80BC24 74010000 00       | cmp byte ptr ss:[esp+174],0             |
77BFDEF1 | 8B7424 14                | mov esi,dword ptr ss:[esp+14]           | [esp+14]:L"./test.dll"
77BFDEF5 | 75 3C                    | jne ntdll.77BFDF33                      |
77BFDEF7 | 85F6                     | test esi,esi                            | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDEF9 | 78 12                    | js ntdll.77BFDF0D                       |
77BFDEFB | 8B4C24 18                | mov ecx,dword ptr ss:[esp+18]           |
77BFDEFF | 8B5424 1C                | mov edx,dword ptr ss:[esp+1C]           |
77BFDF03 | 8B41 18                  | mov eax,dword ptr ds:[ecx+18]           |
77BFDF06 | 8902                     | mov dword ptr ds:[edx],eax              |
77BFDF08 | E8 F4F4FFFF              | call ntdll.77BFD401                     |
77BFDF0D | F605 C037CD77 09         | test byte ptr ds:[77CD37C0],9           |
77BFDF14 | 0F85 11B70400            | jne ntdll.77C4962B                      |
77BFDF1A | 8BC6                     | mov eax,esi                             | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDF1C | 8B8C24 7C010000          | mov ecx,dword ptr ss:[esp+17C]          |
77BFDF23 | 5F                       | pop edi                                 | edi:&"ALLUSERSPROFILE=C:\\ProgramData"
77BFDF24 | 5E                       | pop esi                                 | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDF25 | 5B                       | pop ebx                                 |
77BFDF26 | 33CC                     | xor ecx,esp                             |
77BFDF28 | E8 436C0200              | call ntdll.77C24B70                     |
77BFDF2D | 8BE5                     | mov esp,ebp                             |
77BFDF2F | 5D                       | pop ebp                                 |
77BFDF30 | C2 1000                  | ret 10                                  |
77BFDF33 | FFB424 28010000          | push dword ptr ss:[esp+128]             |
77BFDF3A | E8 C1080100              | call <ntdll.RtlReleasePath>             |
77BFDF3F | EB B6                    | jmp ntdll.77BFDEF7                      |
77BFDF41 | 33DB                     | xor ebx,ebx                             |
77BFDF43 | E9 B8FEFFFF              | jmp ntdll.77BFDE00                      |
77BFDF48 | FF7424 24                | push dword ptr ss:[esp+24]              | [esp+24]:"MZ?"
77BFDF4C | E8 3FDBFDFF              | call <ntdll.RtlDeleteBoundaryDescriptor |
77BFDF51 | E9 6EFFFFFF              | jmp ntdll.77BFDEC4                      |
77BFDF56 | 8BFF                     | mov edi,edi                             |
77BFDF58 | 55                       | push ebp                                |
77BFDF59 | 8BEC                     | mov ebp,esp                             |
77BFDF5B | 64:A1 30000000           | mov eax,dword ptr fs:[30]               |
77BFDF61 | 56                       | push esi                                | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDF62 | 57                       | push edi                                | edi:&"ALLUSERSPROFILE=C:\\ProgramData"
77BFDF63 | 8BF2                     | mov esi,edx                             | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDF65 | 8BF9                     | mov edi,ecx                             | edi:&"ALLUSERSPROFILE=C:\\ProgramData"
77BFDF67 | 8B40 50                  | mov eax,dword ptr ds:[eax+50]           |
77BFDF6A | 85C0                     | test eax,eax                            |
77BFDF6C | 0F85 94B10300            | jne ntdll.77C39106                      |
77BFDF72 | B8 8403FE7F              | mov eax,7FFE0384                        |
77BFDF77 | 8038 00                  | cmp byte ptr ds:[eax],0                 |
77BFDF7A | 0F85 A2B10300            | jne ntdll.77C39122                      |
77BFDF80 | 5F                       | pop edi                                 | edi:&"ALLUSERSPROFILE=C:\\ProgramData"
77BFDF81 | 5E                       | pop esi                                 | esi:&"C:\\Users\\Akkuman\\Desktop\\练武场\\test\\build\\windows\\x86\\release\\main.exe"
77BFDF82 | 5D                       | pop ebp                                 |
77BFDF83 | C2 0400                  | ret 4                                   |
```

其中 `77BFDDC0` 是 `LdrLoadDll` 的地址，`77BFDDC0 ~ 77BFDDC0 + 0x100` 的区间内，所有 `call` 调用的下一条指令地址都在栈上，比如 `77BFDDFE` 就会在 `call ntdll.77BFE900` 时压栈，通过搜索栈，替换该地址，即可达到控制程序执行流程的效果


## 相关资料

- [红队开发 - 白加黑自动化生成器](https://mp.weixin.qq.com/s/MZY9S2yv12b6xFcT1hlnEQ)
- [Adaptive DLL Hijacking](https://www.netspi.com/blog/technical/adversary-simulation/adaptive-dll-hijacking/)
