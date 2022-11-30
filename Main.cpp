#include<windows.h>
#include <windowsX.h>
#include <commctrl.h>
#include <tchar.h>
#include"resource.h"
#pragma comment(lib,"comctl32")

#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

PNOTIFYICONDATA pNID;
HWND hTime, hLevel, hDialog, hText;
HWND hModalDialog;		
HICON hIcon;
HANDLE hMutex;
TCHAR str_time[50];

TCHAR lvl1[] = TEXT("аоао оаоа ааоо ооаа оаао аооа");
TCHAR lvl2[] = TEXT("влвл лвлв ввлл ллвв вллв лввл");
TCHAR lvl3[] = TEXT("прпр рпрп ппрр ррпп рппр пррп");
TCHAR lvl4[] = TEXT("кеке екек ккее еекк кеек екке");
TCHAR lvl5[] = TEXT("нгнг гнгн ннгг ггнн нггн гннг");

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
		hTime = GetDlgItem(hWnd, IDC_TIME);
		hLevel = GetDlgItem(hWnd, IDC_LEVEL);
		hText = GetDlgItem(hWnd, IDC_TEXT); 
		hDialog = hWnd;

		// Получим дескриптор экземпляра приложения
		HINSTANCE hInst = GetModuleHandle(NULL);
		hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); // загружаем иконку
		SetClassLong(hDialog, GCL_HICON, LONG(hIcon)); // устанавливаем иконку в главном окне приложения
		//memset(pNID, 0, sizeof(NOTIFYICONDATA)); //Обнуление структуры
		//pNID->cbSize = sizeof(NOTIFYICONDATA); //размер структуры
		//pNID->hIcon = hIcon; //загружаем пользовательскую иконку
		//pNID->hWnd = hWnd; //дескриптор окна, которое будет получать уведомляющие сообщения, ассоциированные с иконкой в трэе.	
		//pNID->uCallbackMessage = WM_ICON;
		//pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;

		TCHAR GUID[] = TEXT("{D99CD3E0-670D-4def-9B74-99FD7E793DFB}");
		hMutex = CreateMutex(NULL, FALSE, GUID);
		DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
		if (dwAnswer == WAIT_TIMEOUT) {
			MessageBox(hWnd, TEXT("Нельзя запускать более одной копии приложения!!!"), TEXT("Тренажер печати на клавиатуре"), MB_OK | MB_ICONINFORMATION);
			EndDialog(hWnd, 0);
		}
	}
	break;

	case WM_COMMAND: {
		if (LPARAM(wParam) == IDC_START) {
			LRESULT result1 = SendDlgItemMessage(hWnd, IDC_RADIO1, BM_GETCHECK, 0, 0);
			LRESULT result2 = SendDlgItemMessage(hWnd, IDC_RADIO2, BM_GETCHECK, 0, 0);
			LRESULT result3 = SendDlgItemMessage(hWnd, IDC_RADIO3, BM_GETCHECK, 0, 0);
			LRESULT result4 = SendDlgItemMessage(hWnd, IDC_RADIO4, BM_GETCHECK, 0, 0);
			LRESULT result5 = SendDlgItemMessage(hWnd, IDC_RADIO5, BM_GETCHECK, 0, 0);

			if (result1 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl1);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("1")));
			}
			if (result2 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl2);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("2")));
			}
			if (result3 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl3);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("3")));
			}
			if (result4 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl4);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("4")));
			}
			if (result5 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl5);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("5")));
			}
		}

	}
	break;

	/*case WM_TIMER:
		
	break;*/

	case WM_SIZE: {
		if (message == SIZE_MINIMIZED) {
			ShowWindow(hWnd, SW_HIDE); // Прячем окно
			Shell_NotifyIcon(NIM_ADD, pNID); // Добавляем иконку в трэй
		}
	}
	break;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}