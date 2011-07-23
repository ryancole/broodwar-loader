#include <windows.h>
#include <stdio.h>
#include "ReportViewClass.h"

int ReportViewClass::InsertColumn(HWND hDlg, int index, int width, LPTSTR text) {
	columninfo.mask = (LVCF_FMT | LVCF_TEXT | LVCF_WIDTH);
	columninfo.fmt = LVCFMT_LEFT;
	columninfo.pszText = text;
	columninfo.cx = width;
	columninfo.cchTextMax = (strlen(text) + 1);

	return ListView_InsertColumn(hDlg, index, &columninfo);
}

bool ReportViewClass::InsertItem(HWND hDlg, LPTSTR filename, LPTSTR state, LPTSTR path) {
	iteminfo.mask = LVIF_TEXT;
	iteminfo.iItem = 1;
	iteminfo.iSubItem = 0;
	iteminfo.pszText = filename;
	iteminfo.cchTextMax = (strlen(filename) + 1);

	int itemIndex = ListView_InsertItem(hDlg, &iteminfo);
	if(itemIndex == -1)
		return false;

	ListView_SetItemText(hDlg, itemIndex, 1, state);
	ListView_SetItemText(hDlg, itemIndex, 2, path);
	return true;
}

void ReportViewClass::GetItemPath(HWND hDlg, int index, LPTSTR path) {
	ListView_GetItemText(hDlg, index, 2, path, MAX_PATH);
}

void ReportViewClass::GetItemState(HWND hDlg, int index, LPTSTR state) {
	ListView_GetItemText(hDlg, index, 1, state, MAX_PATH);
}

void ReportViewClass::UpdateItemState(HWND hDlg, int index, LPTSTR state) {
	ListView_SetItemText(hDlg, index, 1, state);
}

int ReportViewClass::PopulateWithLibraries(HWND hDlg) {
	int rows = 0;
    char szTemp[MAX_PATH], szCurrentDir[MAX_PATH], szSearchParam[MAX_PATH];

	if(!GetCurrentDirectory(MAX_PATH, szCurrentDir))
		return -1;
	strcpy(szSearchParam, szCurrentDir);
	strcat(szSearchParam, "\\lib\\*.dll");
	
	WIN32_FIND_DATA fileData;
	HANDLE hFind = FindFirstFile(szSearchParam, &fileData);
	if(hFind == INVALID_HANDLE_VALUE)
		return -1;

	sprintf(szTemp, "%s\\lib\\%s", szCurrentDir, fileData.cFileName);
	if(!InsertItem(hDlg, fileData.cFileName, "not injecting", szTemp))
		return -1;

	rows++;
	while(FindNextFile(hFind, &fileData) != 0) {
		sprintf(szTemp, "%s\\lib\\%s", szCurrentDir, fileData.cFileName);
		if(!InsertItem(hDlg, fileData.cFileName, "not injecting", szTemp))
			return -1;
		rows++;
	}

	FindClose(hFind);
	return rows;
}