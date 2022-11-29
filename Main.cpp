#include<windows.h>
#include <windowsX.h>
#include <commctrl.h>
#include <tchar.h>
#include"resource.h"
#pragma comment(lib,"comctl32")
#define GCL_HICON (-14)

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

PNOTIFYICONDATA pNID;
HWND hList, hTime, hLevel, hDialog;
HICON hIcon;
TCHAR str_time[50];

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

//DWORD WINAPI ThreadTime(LPVOID lp) {
//	HWND hProgress = (HWND)lp;
//	struct Time {
//		int Minutes = 2;
//		int Seconds = 30;
//	} Time;
//
//	while (TRUE) {
//		wsprintf(str_time, TEXT("%d:%d"), Time.Minutes, Time.Seconds);
//		SetWindowText(hTime, str_time);
//		if (Time.Seconds <= 0) {
//			Time.Seconds = 60;
//			if (Time.Minutes <= 0) {
//				Time.Seconds--;
//				MessageBox(hTime, TEXT("Время вышло :("), TEXT("Это конец"), MB_OK | MB_ICONINFORMATION);
//				return 0;
//			}
//			else {
//				Time.Seconds--;
//				Time.Minutes--;
//			}
//		}
//		else {
//			Time.Seconds--;
//		}
//		Sleep(1000);
//	}
//	return 0;
//}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG: {
		hList = GetDlgItem(hWnd, IDC_LIST1);
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Первый уровень")));
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Второй уровень")));
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Третий уровень")));
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Четвертый уровень")));
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Пятый уровень")));

		hTime = GetDlgItem(hWnd, IDC_TIME); 
		hLevel = GetDlgItem(hWnd, IDC_LEVEL);
		hDialog = hWnd;

		// Получим дескриптор экземпляра приложения
		HINSTANCE hInst = GetModuleHandle(NULL);
		hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); // загружаем иконку
		SetClassLong(hDialog, GCL_HICON, LONG(hIcon)); // устанавливаем иконку в главном окне приложения
		memset(pNID, 0, sizeof(NOTIFYICONDATA)); //Обнуление структуры
		pNID->cbSize = sizeof(NOTIFYICONDATA); //размер структуры
		pNID->hIcon = hIcon; //загружаем пользовательскую иконку
		pNID->hWnd = hWnd; //дескриптор окна, которое будет получать уведомляющие сообщения, ассоциированные с иконкой в трэе.	


	}
	break;

	case WM_COMMAND: {
		if (LPARAM(wParam) == IDC_START) {
			int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (index != LB_ERR) {
				DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
			}
			SetTimer(hTime, 1, 3000, NULL);
		}
	}
	break;

	/*case WM_TIMER:
		
	break;*/

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}