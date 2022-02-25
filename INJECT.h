#pragma once
typedef unsigned (WINAPI* _LoadLibrary)(LPCWSTR lpLibFileName);
typedef unsigned (WINAPI* _GetModuleHandleW)(LPCWSTR lpModuleName);
typedef int		 (WINAPI* _VirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

typedef struct RemoteData
{
	wchar_t				dllName[0xFF]{};//DLL文件路径
	unsigned			EntryPoint;//入口点
	unsigned			hookEntry;//跳转目标地址
	char entry[5];
	_LoadLibrary		_loadlibrary;
	_GetModuleHandleW	_getmodulehandlew;
	_VirtualProtect		_virtualprotect;
}*LPRemoteData;

class INJECT
{
public:
	//启动进程
	BOOL startProcess(CString& gamePath, CString& gameCommandParameter, CString& curPathName, bool paus, PROCESS_INFORMATION& mation);

	//获取入口点
	DWORD GetEntrance(CString& path);

	//创建远程空间
	DWORD CreateRemoteCode(HANDLE hThread, CString& gamePath, wchar_t* dllname);

	//编码远程
	void codeRemoteData(LPRemoteData, wchar_t* dllname);
};
