## 介绍

实现了文章 [红队开发 - 白加黑自动化生成器](https://mp.weixin.qq.com/s/MZY9S2yv12b6xFcT1hlnEQ) 中提到的 Pre-Load Dll 劫持 的 demo

## 尝试

```shell
> xmake
> build/windows/x86/release/inno_updater.exe
```

## 随手记

如果直接采用代码内定义memcpy等等导出函数，会报错 `error C2169: “memcpy”: 内部函数，不能定义`

可以采用def文件的方式来定义，可查看 [preload.def](./preload.def)

vcruntime140.dll 导出表

```
Ordinal 	Rva     	Function Name                        	区段   	Entry Point(中转输出表)	
00000001	0000E2C0	_CreateFrameInfo                     	.text	                  	
00000002	00004540	_CxxThrowException                   	.text	                  	
00000003	0000E8F0	_EH_prolog                           	.text	                  	
00000004	0000E2F0	_FindAndUnlinkFrame                  	.text	                  	
00000005	0000E340	_IsExceptionObjectToBeDestroyed      	.text	                  	
00000006	0000AFDC	_NLG_Dispatch2                       	.text	                  	
00000007	0000CE37	_NLG_Return                          	.text	                  	
00000008	0000AFE6	_NLG_Return2                         	.text	                  	
00000009	00003060	_SetWinRTOutOfMemoryExceptionCallback	.text	                  	
0000000A	000036F0	__AdjustPointer                      	.text	                  	
0000000B	000035C0	__BuildCatchObject                   	.text	                  	
0000000C	00003490	__BuildCatchObjectHelper             	.text	                  	
0000000D	000039C0	__CxxDetectRethrow                   	.text	                  	
0000000E	000037C0	__CxxExceptionFilter                 	.text	                  	
0000000F	0000E410	__CxxFrameHandler                    	.text	                  	
00000010	0000E410	__CxxFrameHandler2                   	.text	                  	
00000011	0000E410	__CxxFrameHandler3                   	.text	                  	
00000012	0000E450	__CxxLongjmpUnwind                   	.text	                  	
00000013	00003B20	__CxxQueryExceptionSize              	.text	                  	
00000014	00003910	__CxxRegisterExceptionObject         	.text	                  	
00000015	00003A10	__CxxUnregisterExceptionObject       	.text	                  	
00000016	00003650	__DestructExceptionObject            	.text	                  	
00000017	00003100	__FrameUnwindFilter                  	.text	                  	
00000018	00003720	__GetPlatformExceptionInfo           	.text	                  	
00000019	00003D10	__RTCastToVoid                       	.text	                  	
0000001A	00003E30	__RTDynamicCast                      	.text	                  	
0000001B	00003DB0	__RTtypeid                           	.text	                  	
0000001C	00003070	__TypeMatch                          	.text	                  	
0000001D	00003790	__current_exception                  	.text	                  	
0000001E	000037A0	__current_exception_context          	.text	                  	
0000001F	0000ADE0	__intrinsic_setjmp                   	.text	                  	
00000020	000037B0	__processing_throw                   	.text	                  	
00000021	0000E940	__report_gsfailure                   	.text	                  	
00000022	00004320	__std_exception_copy                 	.text	                  	
00000023	00004390	__std_exception_destroy              	.text	                  	
00000024	00003CC0	__std_terminate                      	.text	                  	
00000025	000043B0	__std_type_info_compare              	.text	                  	
00000026	00004510	__std_type_info_destroy_list         	.text	                  	
00000027	000043F0	__std_type_info_hash                 	.text	                  	
00000028	00004420	__std_type_info_name                 	.text	                  	
00000029	00002620	__telemetry_main_invoke_trigger      	.text	                  	
0000002A	00002620	__telemetry_main_return_trigger      	.text	                  	
0000002B	00004B90	__unDName                            	.text	                  	
0000002C	00004C30	__unDNameEx                          	.text	                  	
0000002D	00003770	__uncaught_exception                 	.text	                  	
0000002E	00003780	__uncaught_exceptions                	.text	                  	
0000002F	00004A40	__vcrt_GetModuleFileNameW            	.text	                  	
00000030	00004A60	__vcrt_GetModuleHandleW              	.text	                  	
00000031	000049F0	__vcrt_InitializeCriticalSectionEx   	.text	                  	
00000032	00004A70	__vcrt_LoadLibraryExW                	.text	                  	
00000033	0000B1F0	_chkesp                              	.text	                  	
00000034	0000A9B8	_except_handler2                     	.text	                  	
00000035	0000AA88	_except_handler3                     	.text	                  	
00000036	0000B080	_except_handler4_common              	.text	                  	
00000037	00004AF0	_get_purecall_handler                	.text	                  	
00000038	000045B0	_get_unexpected                      	.text	                  	
00000039	0000AEB0	_global_unwind2                      	.text	                  	
0000003A	00003C10	_is_exception_typeof                 	.text	                  	
0000003B	0000AF15	_local_unwind2                       	.text	                  	
0000003C	0000ABC0	_local_unwind4                       	.text	                  	
0000003D	0000AEA0	_longjmpex                           	.text	                  	
0000003E	00004A90	_purecall                            	.text	                  	
0000003F	0000AC96	_seh_longjmp_unwind4                 	.text	                  	
00000040	0000AB91	_seh_longjmp_unwind                  	.text	                  	
00000041	00004AB0	_set_purecall_handler                	.text	                  	
00000042	00004610	_set_se_translator                   	.text	                  	
00000043	0000AE20	_setjmp3                             	.text	                  	
00000044	00002630	longjmp                              	.text	                  	
00000045	0000CE60	memchr                               	.text	                  	
00000046	0000B690	memcmp                               	.text	                  	
00000047	0000CF10	memcpy                               	.text	                  	
00000048	0000D490	memmove                              	.text	                  	
00000049	0000DA10	memset                               	.text	                  	
0000004A	000045D0	set_unexpected                       	.text	                  	
0000004B	0000DB70	strchr                               	.text	                  	
0000004C	0000DCA0	strrchr                              	.text	                  	
0000004D	0000DDE0	strstr                               	.text	                  	
0000004E	000045F0	unexpected                           	.text	                  	
0000004F	00002640	wcschr                               	.text	                  	
00000050	000026E0	wcsrchr                              	.text	                  	
00000051	00002790	wcsstr                               	.text	                  	
```
