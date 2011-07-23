#include "BWLoader.h"

LRESULT APIENTRY ReportViewSubclass(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
    if(uMsg == WM_GETDLGCODE)
		return DLGC_WANTALLKEYS;  

	switch(uMsg) {
		case WM_INITDIALOG: {
					break;
			}

		case WM_CONTEXTMENU: {
				int selected = ListView_GetSelectionMark(global->handles.hReportView);
				if(selected == -1)
					return CallWindowProc(global->handles.wpOrigReportViewProc, hwnd, uMsg, wParam, lParam);

				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				HMENU hmMenu, hmSubMenu;
				hmMenu = LoadMenu(global->handles.appInstance, MAKEINTRESOURCE(IDR_POPUP));
				hmSubMenu = GetSubMenu(hmMenu, 0);

				BOOL nRetID = TrackPopupMenuEx(hmSubMenu, TPM_LEFTALIGN |
				TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD |
				TPM_RIGHTBUTTON, x, y, global->handles.hMainDlg, NULL);

				char buffer[MAX_PATH];
				global->reportview.GetItemState(global->handles.hReportView, selected, buffer);

				switch(nRetID) {
					case IDC_CHANGESTATE: {
						if(!strncmp(buffer, "not injecting", 13)) {
							global->reportview.UpdateItemState(global->handles.hReportView, selected, "will inject");
						} else if(!strncmp(buffer, "will inject", 11)) {
							global->reportview.UpdateItemState(global->handles.hReportView, selected, "not injecting");
						}
						break;
					}
				}

				DestroyMenu(hmSubMenu);
				DestroyMenu(hmMenu);
				break;
		}
	}

    return CallWindowProc(global->handles.wpOrigReportViewProc, hwnd, uMsg, wParam, lParam); 
}

BOOL CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg){
		case WM_INITDIALOG: {
			global->handles.hMainDlg = hDlg;
			global->handles.hReportView = GetDlgItem(hDlg, IDC_LIST);
			global->handles.hAutoDlg = GetDlgItem(hDlg, IDC_AUTO);
			global->handles.wpOrigReportViewProc = (WNDPROC)SetWindowLong(global->handles.hReportView, GWL_WNDPROC, (LONG)ReportViewSubclass);

			global->reportview.InsertColumn(global->handles.hReportView, 1, 100, "Filename");
			global->reportview.InsertColumn(global->handles.hReportView, 2, 100, "State");
			global->reportview.InsertColumn(global->handles.hReportView, 3, 450, "Path");

			global->reportview.rows = global->reportview.PopulateWithLibraries(global->handles.hReportView);
			if(global->reportview.rows == -1) 
				ListView_DeleteAllItems(global->handles.hReportView);
			
			int x;
			char path[MAX_PATH];
			for(x = 0; x <= (global->reportview.rows - 1); x++) {
				global->reportview.GetItemPath(global->handles.hReportView, x, path);
				if(global->fileoutput.LineExists(path)) {
					global->reportview.UpdateItemState(global->handles.hReportView, x, "will inject");
				}
			}

			global->regedit.Open(HKEY_CURRENT_USER, "Software\\BroodwarThreat\\BWLoader", false);
				if(global->regedit.ReadInt("auto_inject", 0) == 1)
					CheckDlgButton(hDlg, IDC_AUTO, BST_CHECKED);
			global->regedit.Close();

			if(global->IsChecked(global->handles.hAutoDlg)) {
				EnableWindow(GetDlgItem(hDlg, IDC_INJECT), false);
				global->inject.InjectLibrary("SWarClass", "host.dat");
			}
			break;
		}

		case WM_COMMAND: {
			switch(LOWORD(wParam)) {
				case IDC_CLOSE: {
					EndDialog(hDlg, 0);
					break;
				}

				case IDC_INJECT: {
					int x;
					int files = 0;
					char buffer[MAX_PATH];

					for(x = 0; x <= (global->reportview.rows - 1); x++) {
						global->reportview.GetItemState(global->handles.hReportView, x, buffer);
						if(!strncmp(buffer, "will inject", 11)) {
							files++;
							
							global->reportview.GetItemPath(global->handles.hReportView, x, buffer);
							if(files == 1)
								global->fileoutput.WriteLine(true, buffer);
							else {
								global->fileoutput.WriteLine(false, "\n");
								global->fileoutput.WriteLine(false, buffer);
							}
						}
					}

					global->inject.InjectLibrary("SWarClass", "host.dat");
					EnableWindow(GetDlgItem(hDlg, IDC_INJECT), false);
					break;
				}

  				case IDC_AUTO: {
					if(HIWORD(wParam) == BN_CLICKED) {
						global->regedit.Open(HKEY_CURRENT_USER, "Software\\BroodwarThreat\\BWLoader", false);
							global->regedit.WriteInt("auto_inject", IsDlgButtonChecked(hDlg, IDC_AUTO));
						global->regedit.Close();
					}
					break;
				}
			}
			break;
		}
	}

	return false;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	global->appdata.lpInitCommCtrl.dwSize = sizeof(INITCOMMONCONTROLSEX);
	global->appdata.lpInitCommCtrl.dwICC = ICC_LISTVIEW_CLASSES;

	if(!InitCommonControlsEx(&global->appdata.lpInitCommCtrl))
		return 0; // InitCommonControlsEx failed

	global->handles.appInstance = hInstance;
	if(!DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDialogProc))
		return 0; // DialogBox failed

	return 0;
}