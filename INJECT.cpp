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

//���������ڵ�
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
//�޸���ڵ���Ϣ
DWORD WINAPI Remotethreadcode(LPRemoteData obj) {
	//��ȡ������ڵ��ַ����ڵ�+��ʼ��ַ��
	unsigned base = obj->_getmodulehandlew(0);
	obj->EntryPoint += base;
	DWORD dect;
	obj->_virtualprotect((LPVOID)obj->EntryPoint, 0x5, PAGE_EXECUTE_READWRITE, &dect);
	char* entrycode = (char*)obj->EntryPoint;
	//������ڵ�
	obj->entry[0] = entrycode[0];
	obj->entry[1] = entrycode[1];
	obj->entry[2] = entrycode[2];
	obj->entry[3] = entrycode[3];
	obj->entry[4] = entrycode[4];
	//�޸���ڵ�
	entrycode[0] = 0xE9;//jmpָ��
	int dis = obj->hookEntry - obj->EntryPoint - 5;
	int* p = (int*)(obj->EntryPoint + 1);
	p[0] = dis;
	return 1;
}

//����dllע��ģ�鲢�ָ���ڵ�
DWORD WINAPI INJECTcode() {
	unsigned addres{ 0xCCCCCCCC };
	LPRemoteData p = (LPRemoteData)addres;
	//dllע��ģ��
	p->_loadlibrary(p->dllName);
	unsigned entryPoint = p->EntryPoint;//��ڵ�
	//�ָ���ڵ�
	char* entry = (char*)p->EntryPoint;
	entry[0] = p->entry[0];
	entry[1] = p->entry[1];
	entry[2] = p->entry[2];
	entry[3] = p->entry[3];
	entry[4] = p->entry[4];
	//��ת����ڵ����ִ�д���
	_asm {
		mov eax, entryPoint
		jmp eax
	}
	return 0;
}

DWORD INJECT::CreateRemoteCode(HANDLE hThread, CString& gamePath, wchar_t* dllname)
{
	LPVOID res = VirtualAllocEx(hThread, 0, 0x3000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);//�����ڴ�
	RemoteData reda{};
	reda.EntryPoint = GetEntrance(gamePath);//��ȡ�����ڵ�
	LPVOID res1 = (LPVOID)((unsigned)res + 0x2000);
	LPVOID res2 = (LPVOID)((unsigned)res + 0x500);
	codeRemoteData(&reda, dllname);//����ģ�麯����ַ
	reda.hookEntry = (unsigned)res;
	WriteProcessMemory(hThread, res1, &reda, sizeof(reda) + 1, 0);//д������res1

	//�޸ĵ�ַ
	unsigned char str[0X200]{};
	memcpy(str, INJECTcode, 0x100);
	for (int i = 0; i < 0x100; i++) {
		unsigned* p = (unsigned*)(str + i);
		if (p[0] == 0xCCCCCCCC) {
			p[0] = (unsigned)res1;
			break;
		}
	}
	//0-200Զ���̴߳���	res==INJECTcode(�ָ̻߳���)
	//500-1000ע�����	res2==Remotethreadcode()
	//2000������			res1(Զ����Ϣ��)

	WriteProcessMemory(hThread, res, str, 0x100, 0);//д�뺯��res
	WriteProcessMemory(hThread, res2, Remotethreadcode, 0x200, 0);//д�뺯��res
	DWORD pid;
	HANDLE han = CreateRemoteThread(hThread, NULL, 0, (LPTHREAD_START_ROUTINE)res2, (void*)res1, 0, &pid);
	WaitForSingleObject(han, INFINITE);
	CloseHandle(han);
	return 0;
}

//����ģ��api
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