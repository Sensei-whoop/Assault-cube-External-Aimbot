#include "proc.h"

DWORD GetProcId(const wchar_t* procname)
{
	DWORD procId = 0;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32W procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32FirstW(hsnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procname))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32NextW(hsnap, &procEntry));
		}
	}
	CloseHandle(hsnap);
	return procId;
}



uintptr_t GetModuleBaseAddress(DWORD procid, const wchar_t* modName) {
	uintptr_t dwBase = 0;
	do {
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procid);
		if (hSnapshot == INVALID_HANDLE_VALUE) { continue; }
		MODULEENTRY32W ModuleEntry32;
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32W);
		if (Module32FirstW(hSnapshot, &ModuleEntry32)) {
			do {
				if (!wcscmp(ModuleEntry32.szModule, modName)) {
					dwBase = (uintptr_t)ModuleEntry32.modBaseAddr;
					break;
				}
			} while (Module32NextW(hSnapshot, &ModuleEntry32));
		}
		CloseHandle(hSnapshot);
	} while (!dwBase);
	return dwBase;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		//add each offset to the address
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}
