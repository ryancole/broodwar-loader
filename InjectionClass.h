#pragma once

#include <windows.h>

class InjectionClass {
public:
	bool InjectLibrary(LPCTSTR className, LPCTSTR library);
};