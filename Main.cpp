#include<windows.h>
#include <windowsX.h>
#include <commctrl.h>
#include <tchar.h>
#include <conio.h>
#include <time.h>
#include"resource.h"
#pragma comment(lib,"comctl32")

#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

PNOTIFYICONDATA pNID;
HWND hTime, hLevel, hDialog, hText, hEnterText, hFinish;
HWND hEdit1, hEdit2, hEdit3;
HWND hModalDialog;	
HICON hIcon;
HANDLE hMutex;
HANDLE hThread;
TCHAR str_time[50];
TCHAR str[100];

TCHAR lvl1[] = TEXT("аоао оаоа ааоо ооаа оаао аооа");
TCHAR lvl2[] = TEXT("влвл лвлв ввлл ллвв вллв лввл");
TCHAR lvl3[] = TEXT("прпр рпрп ппрр ррпп рппр пррп");
TCHAR lvl4[] = TEXT("кеке екек ккее еекк кеек екке");
TCHAR lvl5[] = TEXT("нгнг гнгн ннгг ггнн нггн гннг");

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

void OnTrayIcon(WPARAM wp, LPARAM lp) {
	if (lp == WM_LBUTTONDBLCLK) {
		Shell_NotifyIcon(NIM_DELETE, pNID); // Удаляем иконку из трэя
		ShowWindow(hDialog, SW_NORMAL); // Восстанавливаем окно
		SetForegroundWindow(hDialog); // устанавливаем окно на передний план
	}
}

void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy) {
	if (state == SIZE_MINIMIZED) {
		ShowWindow(hwnd, SW_HIDE); // Прячем окно
		Shell_NotifyIcon(NIM_ADD, pNID); // Добавляем иконку в трэй
	}
}

DWORD WINAPI TimeFunc(LPVOID lp) {
	HWND hProgress = (HWND)lp;
	struct Time {
		int Minutes = 0;
		int Seconds = 0;
	} Time;

	while (TRUE) {
		wsprintf(str_time, TEXT("%d:%d"), Time.Minutes, Time.Seconds);
		SetWindowText(hTime, str_time);
		if (Time.Seconds >= 60) {
			Time.Seconds = 00;
			Time.Minutes++;
		}
		else {
			Time.Seconds++;
		}
		Sleep(1000);
	}
	return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG: {
		hTime = GetDlgItem(hWnd, IDC_TIME);
		hEdit1 = GetDlgItem(hWnd, IDC_EDIT1);
		hLevel = GetDlgItem(hWnd, IDC_LEVEL);
		hText = GetDlgItem(hWnd, IDC_TEXT); 
		hEnterText = GetDlgItem(hWnd, IDC_ENTERTEXT);
		hFinish = GetDlgItem(hWnd, IDOK);
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
		//pNID->uID = ID_TRAYICON;


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

			hThread = CreateThread(NULL, 0, TimeFunc, hTime, 0, NULL);
		}

		if (LPARAM(wParam) == IDOK) {
			CloseHandle(hThread);
			EndDialog(hModalDialog, 0);
			hWnd = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DLGPROC(DlgProc));
			ShowWindow(hWnd, SW_RESTORE);
			SendMessage(hEdit1, WM_SETTEXT, 0, LPARAM(str_time));
		}

	}
	break;

	//case WM_SIZE: {
	//	if (message == SIZE_MINIMIZED) {
	//		ShowWindow(hWnd, SW_HIDE); // Прячем окно
	//		Shell_NotifyIcon(NIM_ADD, pNID); // Добавляем иконку в трэй
	//	}
	//}
	//break;

	HANDLE_MSG(hWnd, WM_SIZE, Cls_OnSize);

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	if (message == WM_ICON) {
		OnTrayIcon(wParam, lParam);
		return TRUE;
	}
	return FALSE;
}