#include "includes.h"

HANDLE handle;
HANDLE snapshot;


DWORD getProcessIdFromWindowName(const char* windowName) {
	//Find window
	HWND hwnd = NULL;

	while (hwnd == NULL) {
		hwnd = FindWindowA(NULL, windowName);
		Sleep(1000);
	}

	//GetProcessID and handle
	DWORD processID;
	//Store proces id in the variable
	GetWindowThreadProcessId(hwnd, &processID);

	return processID;
}

HANDLE getHandleFromProcessID(DWORD processID) {
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	return handle;
}

DWORD getModule(const char* moduleName, DWORD processID) {
	// Take a snapshot of all modules inside a given process.
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
	MODULEENTRY32 moduleEntry;

	moduleEntry.dwSize = sizeof(moduleEntry);

	DWORD moduleAddr;

	do {
		if (!strcmp(moduleEntry.szModule, moduleName)) {
			moduleAddr = (DWORD)moduleEntry.modBaseAddr;
		}
	} while (Module32Next(snapshot, &moduleEntry));

	return moduleAddr;
}

template <class T>

T readMem(DWORD start, DWORD offset) {
	T result;
	ReadProcessMemory(handle, (LPVOID)(start + offset), &result, sizeof(result), NULL);
	return (result);
}

template <class T>

T writeMem(DWORD start, DWORD offset, T toWrite) {
	WriteProcessMemory(handle, LPVOID(start + offset), &toWrite, sizeof(toWrite), NULL);
	return 0;
}
