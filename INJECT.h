#pragma once
typedef unsigned (WINAPI* _LoadLibrary)(LPCWSTR lpLibFileName);
typedef unsigned (WINAPI* _GetModuleHandleW)(LPCWSTR lpModuleName);
typedef int		 (WINAPI* _VirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

typedef struct RemoteData
{
	wchar_t				dllName[0xFF]{};//DLL�ļ�·��
	unsigned			EntryPoint;//��ڵ�
	unsigned			hookEntry;//��תĿ���ַ
	char entry[5];
	_LoadLibrary		_loadlibrary;
	_GetModuleHandleW	_getmodulehandlew;
	_VirtualProtect		_virtualprotect;
}*LPRemoteData;

class INJECT
{
public:
	//��������
	BOOL startProcess(CString& gamePath, CString& gameCommandParameter, CString& curPathName, bool paus, PROCESS_INFORMATION& mation);

	//��ȡ��ڵ�
	DWORD GetEntrance(CString& path);

	//����Զ�̿ռ�
	DWORD CreateRemoteCode(HANDLE hThread, CString& gamePath, wchar_t* dllname);

	//����Զ��
	void codeRemoteData(LPRemoteData, wchar_t* dllname);
};
