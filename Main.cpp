#include "Main.h"

#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

PNOTIFYICONDATA pNID;

HWND hTime, hLevel, hDialog, hText, hEnterText, hFinish;
HWND hEdit1, hEdit2, hEdit3;
HWND hButtonA, hButtonO, hButtonV, hButtonL, hButtonP, hButtonR, hButtonK, hButtonE, hButtonN, hButtonG, hSpace;
HWND hModalDialog, hResultDialog;

HICON hIcon;
HHOOK hHook;

HANDLE hMutex;
HANDLE hThread, hThreadBtclk;

TCHAR str_time[50];
TCHAR str_result[100];
TCHAR str_text[50];
TCHAR str_timer[50];
TCHAR str_button[3];

int points = 0;
int error = 0;

TCHAR lvl1[] = TEXT("аоао оаоа ааоо ооаа оаао аооа");
TCHAR lvl2[] = TEXT("влвл лвлв ввлл ллвв вллв лввл");
TCHAR lvl3[] = TEXT("прпр рпрп ппрр ррпп рппр пррп");
TCHAR lvl4[] = TEXT("кеке екек ккее еекк кеек екке");
TCHAR lvl5[] = TEXT("нгнг гнгн ннгг ггнн нггн гннг");
TCHAR str_lvl[10];

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

void Check_Level_1(TCHAR *buf, int i) {
	if (buf[i] == 'F') {
		Button_Enable(hButtonA, FALSE);
		Button_Enable(hButtonO, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == 'J') {
		Button_Enable(hButtonO, FALSE);
		Button_Enable(hButtonA, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == ' ') {
		Button_Enable(hSpace, FALSE);
		Button_Enable(hButtonA, TRUE);
		Button_Enable(hButtonO, TRUE);
	}
}
void Check_Level_2(TCHAR* buf, int i) {
	if (buf[i] == 'D') {
		Button_Enable(hButtonV, FALSE);
		Button_Enable(hButtonL, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == 'K') {
		Button_Enable(hButtonL, FALSE);
		Button_Enable(hButtonV, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == ' ') {
		Button_Enable(hButtonL, TRUE);
		Button_Enable(hButtonV, TRUE);
		Button_Enable(hSpace, FALSE);
	}
}
void Check_Level_3(TCHAR* buf, int i) {
	if (buf[i] == 'G') {
		Button_Enable(hButtonP, FALSE);
		Button_Enable(hButtonR, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == 'H') {
		Button_Enable(hButtonR, FALSE);
		Button_Enable(hButtonP, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == ' ') {
		Button_Enable(hButtonP, TRUE);
		Button_Enable(hButtonR, TRUE);
		Button_Enable(hSpace, FALSE);
	}
}
void Check_Level_4(TCHAR* buf, int i) {
	if (buf[i] == 'R') {
		Button_Enable(hButtonK, FALSE);
		Button_Enable(hButtonE, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == 'T') {
		Button_Enable(hButtonE, FALSE);
		Button_Enable(hButtonK, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == ' ') {
		Button_Enable(hButtonE, TRUE);
		Button_Enable(hButtonK, TRUE);
		Button_Enable(hSpace, FALSE);
	}
}
void Check_Level_5(TCHAR* buf, int i) {
	if (buf[i] == 'Y') {
		Button_Enable(hButtonN, FALSE);
		Button_Enable(hButtonG, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == 'U') {
		Button_Enable(hButtonG, FALSE);
		Button_Enable(hButtonN, TRUE);
		Button_Enable(hSpace, TRUE);
	}
	else if (buf[i] == ' ') {
		Button_Enable(hButtonN, TRUE);
		Button_Enable(hButtonG, TRUE);
		Button_Enable(hSpace, FALSE);
	}
}

LRESULT CALLBACK KeyboardProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == idCode) {
		HWND hWnd = GetFocus();
		TCHAR buf[40], text[65536];
		GetClassName(hWnd, buf, 40);
		if (lstrcmpi(buf, L"EDIT") == 0 && (lParam & 0x80000000)) {
			int i = 0;
			if (!(wParam >= 'А' && wParam <= 'Я' || wParam == ' ')) {
				wsprintf(buf, TEXT("%c"), wParam);
			}
			Check_Level_1(buf, i);
			Check_Level_2(buf, i);
			Check_Level_3(buf, i);
			Check_Level_4(buf, i);
			Check_Level_5(buf, i);
			i++;			
		}
	}
	return CallNextHookEx(hHook, idCode, wParam, lParam);
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
		hEdit2 = GetDlgItem(hWnd, IDC_EDIT2);
		hEdit3 = GetDlgItem(hWnd, IDC_EDIT3);
		hLevel = GetDlgItem(hWnd, IDC_LEVEL);
		hText = GetDlgItem(hWnd, IDC_TEXT); 
		hEnterText = GetDlgItem(hWnd, IDC_ENTERTEXT);
		hFinish = GetDlgItem(hWnd, IDOK);
		hDialog = hWnd;

		hButtonA = GetDlgItem(hWnd, IDC_BUTTON16);
		hButtonO = GetDlgItem(hWnd, IDC_BUTTON19);
		hButtonV = GetDlgItem(hWnd, IDC_BUTTON15);
		hButtonL = GetDlgItem(hWnd, IDC_BUTTON20);
		hButtonP = GetDlgItem(hWnd, IDC_BUTTON17);
		hButtonR = GetDlgItem(hWnd, IDC_BUTTON18);
		hButtonK = GetDlgItem(hWnd, IDC_BUTTON4);
		hButtonE = GetDlgItem(hWnd, IDC_BUTTON5);
		hButtonN = GetDlgItem(hWnd, IDC_BUTTON6);
		hButtonG = GetDlgItem(hWnd, IDC_BUTTON7);
		hSpace = GetDlgItem(hWnd, IDC_SPACE);

		pNID = new NOTIFYICONDATA;
		HINSTANCE hInst = GetModuleHandle(NULL); // Получим дескриптор экземпляра приложения
		hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
		SetClassLong(hDialog, GCL_HICON, LONG(hIcon)); // устанавливаем иконку в главном окне приложения
		pNID->cbSize = sizeof(NOTIFYICONDATA); //размер структуры
		memset(pNID, 0, sizeof(NOTIFYICONDATA)); //Обнуление структуры
		pNID->hIcon = hIcon; //загружаем пользовательскую иконку
		pNID->hWnd = hDialog; //дескриптор окна, которое будет получать уведомляющие сообщения, ассоциированные с иконкой в трэе.	
		pNID->uCallbackMessage = WM_ICON;
		pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
		pNID->uID = ID_TRAYICON;


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
				wsprintf(str_lvl, TEXT("%s"), TEXT("1"));
			}
			if (result2 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl2);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("2")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("2"));
			}
			if (result3 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl3);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("3")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("3"));
			}
			if (result4 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl4);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("4")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("4"));
			}
			if (result5 == BST_CHECKED) {
				hModalDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DLGPROC(DlgProc));
				ShowWindow(hModalDialog, SW_RESTORE);
				SetWindowText(hText, lvl5);
				SendMessage(hLevel, WM_SETTEXT, 0, LPARAM(TEXT("5")));
				wsprintf(str_lvl, TEXT("%s"), TEXT("5"));
			}

			hThread = CreateThread(NULL, 0, TimeFunc, hTime, 0, NULL);

			TCHAR str_1[10] = TEXT("1");
			TCHAR str_2[10] = TEXT("2");
			TCHAR str_3[10] = TEXT("3");
			TCHAR str_4[10] = TEXT("4");
			TCHAR str_5[10] = TEXT("5");

			int compare = _tcscmp(str_lvl, str_1);
			if (compare == 0) {
				wsprintf(str_text, TEXT("%s"), lvl1);
			}
			else {
				compare = _tcscmp(str_lvl, str_2);
				if (compare == 0) {
					wsprintf(str_text, TEXT("%s"), lvl2);
				}
				else {
					compare = _tcscmp(str_lvl, str_3);
					if (compare == 0) {
						wsprintf(str_text, TEXT("%s"), lvl3);
					}
					else {
						compare = _tcscmp(str_lvl, str_4);
						if (compare == 0) {
							wsprintf(str_text, TEXT("%s"), lvl4);
						}
						else {
							compare = _tcscmp(str_lvl, str_5);
							wsprintf(str_text, TEXT("%s"), lvl5);
						}
					}
				}
			}

			hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
		}

		if (LPARAM(wParam) == IDOK) {
			points = 0;
			error = 0;

			int count_result = GetWindowTextLength(hEnterText);
			GetWindowText(hEnterText, str_result, count_result + 1);
			for (int i = 0; i < 30; i++) {
				if (str_result[i] == str_text[i]) {
					points++;
				}
				else {
					error++;
				}
			}
			TCHAR str_points[10];
			wsprintf(str_points, TEXT("%d"), points);
			TCHAR str_error[10];
			wsprintf(str_error, TEXT("%d"), error);

			CloseHandle(hThread);
			EndDialog(hModalDialog, 0);
			hResultDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hWnd, DLGPROC(DlgProc));
			ShowWindow(hResultDialog, SW_RESTORE);
			SendMessage(hEdit1, WM_SETTEXT, 0, LPARAM(str_time));
			SendMessage(hEdit2, WM_SETTEXT, 0, LPARAM(str_error));
			SendMessage(hEdit3, WM_SETTEXT, 0, LPARAM(str_points));
		}
	}
	break;

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