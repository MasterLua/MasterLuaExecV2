#include "dllmain.h"
#include "auth.h"

ScriptHook t;

void ok()
{
	string hostfile = "http://dev.masterlua.com:8189/covid19/masterlua/exec?chingchong=";
	string hwid = a_gethid();
	string result = a_DownloadURL(hostfile + hwid);
	if (result == "P6RdBml8SzgltZYZ") {
	}
	else {
		MessageBoxA(NULL, "Anti Crack Activate", "MasterLuaExec", NULL);
		Sleep(2000);
		exit(1);
	}
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // 进程加载
		CreateThread(NULL, 0, ThreadFunc, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH: // 进程卸载
		t.Release();
		break;
	}
	return TRUE;
}

DWORD WINAPI ThreadFunc(LPVOID)
{
	ok();
	t.Initialize();
	return 0;
}