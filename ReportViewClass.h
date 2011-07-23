#pragma once

#include <commctrl.h>

class ReportViewClass {
private:
	LVCOLUMN columninfo;
	LVITEM iteminfo;
public:
	int rows;

	int InsertColumn(HWND hDlg, int index, int width, LPTSTR text);
	bool InsertItem(HWND hDlg, LPTSTR filename, LPTSTR state, LPTSTR path);
	void GetItemPath(HWND hDlg, int index, LPTSTR path);
	void GetItemState(HWND hDlg, int index, LPTSTR state);
	void UpdateItemState(HWND hDlg, int index, LPTSTR state);
	int PopulateWithLibraries(HWND hDlg);
};