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

TCHAR lvl1[] = TEXT("���� ���� ���� ���� ���� ����");
TCHAR lvl2[] = TEXT("���� ���� ���� ���� ���� ����");
TCHAR lvl3[] = TEXT("���� ���� ���� ���� ���� ����");
TCHAR lvl4[] = TEXT("���� ���� ���� ���� ���� ����");
TCHAR lvl5[] = TEXT("���� ���� ���� ���� ���� ����");

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

void OnTrayIcon(WPARAM wp, LPARAM lp) {
	if (lp == WM_LBUTTONDBLCLK) {
		Shell_NotifyIcon(NIM_DELETE, pNID); // ������� ������ �� ����
		ShowWindow(hDialog, SW_NORMAL); // ��������������� ����
		SetForegroundWindow(hDialog); // ������������� ���� �� �������� ����
	}
}

void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy) {
	if (state == SIZE_MINIMIZED) {
		ShowWindow(hwnd, SW_HIDE); // ������ ����
		Shell_NotifyIcon(NIM_ADD, pNID); // ��������� ������ � ����
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

		// ������� ���������� ���������� ����������
		HINSTANCE hInst = GetModuleHandle(NULL);
		hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); // ��������� ������
		SetClassLong(hDialog, GCL_HICON, LONG(hIcon)); // ������������� ������ � ������� ���� ����������
		//memset(pNID, 0, sizeof(NOTIFYICONDATA)); //��������� ���������
		//pNID->cbSize = sizeof(NOTIFYICONDATA); //������ ���������
		//pNID->hIcon = hIcon; //��������� ���������������� ������
		//pNID->hWnd = hWnd; //���������� ����, ������� ����� �������� ������������ ���������, ��������������� � ������� � ����.	
		//pNID->uCallbackMessage = WM_ICON;
		//pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
		//pNID->uID = ID_TRAYICON;


		TCHAR GUID[] = TEXT("{D99CD3E0-670D-4def-9B74-99FD7E793DFB}");
		hMutex = CreateMutex(NULL, FALSE, GUID);
		DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
		if (dwAnswer == WAIT_TIMEOUT) {
			MessageBox(hWnd, TEXT("������ ��������� ����� ����� ����� ����������!!!"), TEXT("�������� ������ �� ����������"), MB_OK | MB_ICONINFORMATION);
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
	//		ShowWindow(hWnd, SW_HIDE); // ������ ����
	//		Shell_NotifyIcon(NIM_ADD, pNID); // ��������� ������ � ����
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