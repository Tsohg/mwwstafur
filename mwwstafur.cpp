#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "D:\REKIT\GH Injector V3.3\GH Injector V3.3 - Source\GH Injector Library\GH Injector Library\Injection.h"

using namespace std;

//Starts mww's process and returns the pid.
DWORD startmww(char *path) //argv[1] should be a pointer to a char[]?
{
	char args[] = "-bundle-dir ./data_win32_bundled -window-title AliasSandbox -ini settings_client -property-order client server -network-profile internet -disable-persistence";

	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	LPCWSTR lpcPath;
	LPWSTR lpcArgs;

	wchar_t buf[MAX_PATH];
	size_t outSize;

	mbstowcs_s(&outSize, buf, args, strlen(args) + 1);
	lpcArgs = (LPWSTR)buf;

	//memory allocation
	ZeroMemory(&sInfo, sizeof(sInfo));
	sInfo.cb = sizeof(sInfo);
	ZeroMemory(&pInfo, sizeof(pInfo));
	ZeroMemory(buf, sizeof(buf));

	outSize = 0;
	mbstowcs_s(&outSize, buf, path, strlen(path) + 1);
	lpcPath = buf;

	BOOL success = CreateProcess(
		lpcPath,
		lpcArgs,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		NULL,
		&sInfo,
		&pInfo
	);
	if (!success)
		return -1;
	return pInfo.dwProcessId;
}
// exe D:\Disk Drive Downloads\Steam\steamapps\common\MagickaWizardWars\bitsquid_win32_dev.exe C:\Users\Nathan\source\repos\stafur\Debug\stafur.dll D:\REKIT\GH Injector V3.3\GH Injector - x64.dll
//argv[1] = MWW's Path
//argv[2] = stafur DLL path
//argv[3] = GH injector dll path

typedef DWORD (*InjectionA)(INJECTIONDATAA*);

int main(int argc, char *argv[])
{
	char* path = argv[1];
	DWORD mwwPid = startmww(path);
	INJECTIONDATAA idata;
	DWORD lastError = -1;

	idata.LastErrorCode = lastError;
	strncpy_s(idata.szDllPath, argv[2], strlen(argv[2]));
	idata.szDllPath[strlen(argv[2])] = '\0';
	idata.ProcessID = mwwPid;
	idata.hDllOut = NULL;
	idata.hHandleValue = NULL;
	idata.Method = LM_NtCreateThreadEx;
	idata.Mode = IM_LoadLibrary;
	idata.Flags = 0;

	HMODULE injdll = LoadLibraryA(argv[3]);
	void* procAdd = GetProcAddress(injdll, "InjectA");
	InjectionA injA = reinterpret_cast<InjectionA>(procAdd);
	injA(&idata);
}
