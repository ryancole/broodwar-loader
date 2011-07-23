#include <stdio.h>
#include "InjectionClass.h"

bool InjectionClass::InjectLibrary(LPCTSTR className, LPCTSTR library) {
    HINSTANCE hDll = LoadLibrary(library);
	if(hDll == NULL)
		return false;

    HOOKPROC procAddr = (HOOKPROC)GetProcAddress(hDll, "CBTProc");
	if(procAddr == NULL)
		return false;

	HWND windowId = FindWindow(className, NULL);
	if(windowId == NULL)
		return false;

    SetWindowsHookEx(WH_CBT, procAddr, hDll, GetWindowThreadProcessId(windowId, NULL));
    return true;
}