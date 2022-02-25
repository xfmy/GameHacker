#include "pch.h"
#include "INJECT.h"
#include <ImageHlp.h>
#pragma comment(lib,"imagehlp.lib")
#include <fstream>
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS

BOOL INJECT::startProcess(CString& gamePath, CString& gameCommandParameter, CString& curPathName, bool paus, PROCESS_INFORMATION& mation)
{
	STARTUPINFOW info{};
	info.cb = sizeof info;
	DWORD value = CREATE_SUSPENDED;
	if (paus == true) value = 0;
	bool a = CreateProcessW(curPathName, gameCommandParameter.GetBuffer(), NULL, NULL, FALSE, value, NULL, gamePath, &info, &mation);
	int b;
	if (a == 0)
	{
		b = GetLastError();
	}
	return 0;
	//return CreateProcessW(gamePath, gameCommandParameter.GetBuffer(), NULL, NULL, FALSE, value, NULL, curPathName, &info, &mation);
}

//计算相对入口点
DWORD INJECT::GetEntrance(CString& path)
{
	std::ifstream file(path, std::ios::binary);
	file.seekg(0, std::ios::end);
	unsigned size = file.tellg();
	char* filesize = new char[size];
	file.seekg(0, std::ios::beg);
	file.read(filesize, size);
	file.close();
	IMAGE_DOS_HEADER* dosheader = (IMAGE_DOS_HEADER*)filesize;
	unsigned ped = dosheader->e_lfanew + (unsigned)filesize;
	IMAGE_NT_HEADERS* ntheadr = (IMAGE_NT_HEADERS*)ped;
	DWORD dent = ntheadr->OptionalHeader.AddressOfEntryPoint;
	delete[] filesize;
	return dent;
}
//修改入口点信息
DWORD WINAPI Remotethreadcode(LPRemoteData obj) {
	//获取绝对入口点地址（入口点+起始地址）
	unsigned base = obj->_getmodulehandlew(0);
	obj->EntryPoint += base;
	DWORD dect;
	obj->_virtualprotect((LPVOID)obj->EntryPoint, 0x5, PAGE_EXECUTE_READWRITE, &dect);
	char* entrycode = (char*)obj->EntryPoint;
	//保存入口点
	obj->entry[0] = entrycode[0];
	obj->entry[1] = entrycode[1];
	obj->entry[2] = entrycode[2];
	obj->entry[3] = entrycode[3];
	obj->entry[4] = entrycode[4];
	//修改入口点
	entrycode[0] = 0xE9;//jmp指令
	int dis = obj->hookEntry - obj->EntryPoint - 5;
	int* p = (int*)(obj->EntryPoint + 1);
	p[0] = dis;
	return 1;
}

//加载dll注入模块并恢复入口点
DWORD WINAPI INJECTcode() {
	unsigned addres{ 0xCCCCCCCC };
	LPRemoteData p = (LPRemoteData)addres;
	//dll注入模块
	p->_loadlibrary(p->dllName);
	unsigned entryPoint = p->EntryPoint;//入口点
	//恢复入口点
	char* entry = (char*)p->EntryPoint;
	entry[0] = p->entry[0];
	entry[1] = p->entry[1];
	entry[2] = p->entry[2];
	entry[3] = p->entry[3];
	entry[4] = p->entry[4];
	//跳转回入口点继续执行代码
	_asm {
		mov eax, entryPoint
		jmp eax
	}
	return 0;
}

DWORD INJECT::CreateRemoteCode(HANDLE hThread, CString& gamePath, wchar_t* dllname)
{
	LPVOID res = VirtualAllocEx(hThread, 0, 0x3000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);//创建内存
	RemoteData reda{};
	reda.EntryPoint = GetEntrance(gamePath);//获取相对入口点
	LPVOID res1 = (LPVOID)((unsigned)res + 0x2000);
	LPVOID res2 = (LPVOID)((unsigned)res + 0x500);
	codeRemoteData(&reda, dllname);//加载模块函数地址
	reda.hookEntry = (unsigned)res;
	WriteProcessMemory(hThread, res1, &reda, sizeof(reda) + 1, 0);//写入数据res1

	//修改地址
	unsigned char str[0X200]{};
	memcpy(str, INJECTcode, 0x100);
	for (int i = 0; i < 0x100; i++) {
		unsigned* p = (unsigned*)(str + i);
		if (p[0] == 0xCCCCCCCC) {
			p[0] = (unsigned)res1;
			break;
		}
	}
	//0-200远程线程代码	res==INJECTcode(线程恢复类)
	//500-1000注入代码	res2==Remotethreadcode()
	//2000数据区			res1(远程信息类)

	WriteProcessMemory(hThread, res, str, 0x100, 0);//写入函数res
	WriteProcessMemory(hThread, res2, Remotethreadcode, 0x200, 0);//写入函数res
	DWORD pid;
	HANDLE han = CreateRemoteThread(hThread, NULL, 0, (LPTHREAD_START_ROUTINE)res2, (void*)res1, 0, &pid);
	WaitForSingleObject(han, INFINITE);
	CloseHandle(han);
	return 0;
}

//加载模块api
void INJECT::codeRemoteData(LPRemoteData reda, wchar_t* dllname)
{
	HMODULE hmo = LoadLibraryW(L"kernel32.dll");
	short size;
	for (size = 0; dllname[size]; size++);
	reda->_loadlibrary = (_LoadLibrary)GetProcAddress(hmo, "LoadLibraryW");
	reda->_getmodulehandlew = (_GetModuleHandleW)GetProcAddress(hmo, "GetModuleHandleW");
	reda->_virtualprotect = (_VirtualProtect)GetProcAddress(hmo, "VirtualProtect");
	memcpy(reda->dllName, dllname, (size + 1) * 2);
}