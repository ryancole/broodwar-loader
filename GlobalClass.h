#pragma once

#include <commctrl.h>
#include "ReportViewClass.h"
#include "RegistryClass.h"
#include "FileClass.h"
#include "InjectionClass.h"

struct GlobalHandles {
	HINSTANCE appInstance;
	HWND hMainDlg, hReportView, hAutoDlg;
	WNDPROC wpOrigReportViewProc;
};

struct ApplicationData {
	INITCOMMONCONTROLSEX lpInitCommCtrl;
};

class GlobalClass {
public:
	ApplicationData appdata;
	GlobalHandles handles;
	ReportViewClass reportview;
	CRegisterWIN32 regedit;
	FileClass fileoutput;
	InjectionClass inject;

	inline bool IsChecked(HWND hDlg) { if(SendMessage(hDlg, BM_GETCHECK, 0, 0) == BST_CHECKED) return true; else return false; };
};

extern GlobalClass *global;