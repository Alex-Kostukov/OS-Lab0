#include "framework.h"
#include "WindowsProject1.h"
#include <cmath>
#include <string>

#define MAX_LOADSTRING 100

using namespace std;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HBITMAP HBitMap1;
HWND HButton[6], HStatic, HScroll, WindowTask1, DlCombo1, DlCombo2, DlList1, Hmatrix[5][5], Hmatrixb[5], hList;
HMENU HPopMenu;
int cur;
double Amatrix[5][5], Bmatrix[5];
double* x;
int len1, len2;


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    WindowTask(HWND, UINT, WPARAM, LPARAM);
void ChangeInf(HWND hd);
void FindX(HWND hDlg,double a[5][5], double b[5], int cur1);
double* gauss(HWND hDlg,double a[5][5], double y[5], int n);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR2));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      10, 10, 500, 300, nullptr, nullptr, hInstance, nullptr);

   HButton[0] = CreateWindowW(L"button", L"Push1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       210, 10, 60, 30, hWnd, nullptr, hInstance, NULL);
   HButton[1] = CreateWindowW(L"button", L"Push2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       280, 10, 60, 30, hWnd, nullptr, hInstance, NULL);
   HButton[2] = CreateWindowW(L"button", L"Push3", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       350, 10, 60, 30, hWnd, nullptr, hInstance, NULL);

   HButton[3] = CreateWindowW(L"button", L"Rad1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
       210, 50, 60, 30, hWnd, nullptr, hInstance, NULL);
   HButton[4] = CreateWindowW(L"button", L"Rad2", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       280, 50, 60, 30, hWnd, nullptr, hInstance, NULL);
   HButton[5] = CreateWindowW(L"button", L"Rad3", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       350, 50, 60, 30, hWnd, nullptr, hInstance, NULL);

   HStatic = CreateWindowW(L"static", L"Гаусс", WS_CHILD | WS_VISIBLE,
       210, 90, 60, 30, hWnd, nullptr, hInstance, NULL);

   HScroll = CreateWindow(L"edit", L"Гаусс1 Гаусс2 Гаусс3 Гаусс4 Гаусс5 Гаусс6 Гаусс7", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
       210, 90, 100, 60, hWnd, nullptr, hInstance, NULL);


   HBitMap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
   HPopMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU1));

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_RBUTTONDOWN:
    {
        HMENU menu = GetSubMenu(HPopMenu, 0);
        LPPOINT point = new POINT();
        point->x = LOWORD(lParam);
        point->y = HIWORD(lParam);
        ClientToScreen(hWnd, point);

        TrackPopupMenu(menu, TPM_RIGHTBUTTON, point->x, point->y, 0, hWnd, NULL);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_POP_TASK:
                WindowTask1 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, WindowTask);
                ShowWindow(WindowTask1, SW_SHOW);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HDC h = CreateCompatibleDC(hdc);
            BITMAP bm;
            SelectObject(h, HBitMap1);
            GetObject(HBitMap1, sizeof(BITMAP), &bm);
            StretchBlt(hdc, 0, 0, 200, 200, h, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
            DeleteDC(h);
            EndPaint(hWnd, &ps);
            return 0;

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK WindowTask(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

    case WM_INITDIALOG:
    {
        cur = -1;
        DlCombo1 = GetDlgItem(hDlg, IDC_COMBO1);
        SendMessage(DlCombo1, CB_ADDSTRING, 0, (LPARAM)"1");
        SendMessage(DlCombo1, CB_ADDSTRING, 0, (LPARAM)"2");
        SendMessage(DlCombo1, CB_SETCURSEL, 0, 0);

        DlList1 = GetDlgItem(hDlg, IDC_LIST1);
        SendMessage(DlList1, LB_ADDSTRING, 0, (LPARAM)"1");
        SendMessage(DlList1, LB_ADDSTRING, 0, (LPARAM)"2");
        SendMessage(DlList1, LB_SETCURSEL, 0, 0);

        DlCombo2 = GetDlgItem(hDlg, IDC_COMBO2);
        SendMessage(DlCombo2, CB_ADDSTRING, 0, (LPARAM)"1");
        SendMessage(DlCombo2, CB_ADDSTRING, 0, (LPARAM)"2");
        SendMessage(DlCombo2, CB_ADDSTRING, 0, (LPARAM)"3");
        SendMessage(DlCombo2, CB_ADDSTRING, 0, (LPARAM)"4");
        SendMessage(DlCombo2, CB_ADDSTRING, 0, (LPARAM)"5");
        SendMessage(DlCombo2, CB_SETCURSEL, 0, 0);
        cur = 0;

        Hmatrix[0][0] = GetDlgItem(hDlg, IDC_EDIT2);
        Hmatrix[0][1] = GetDlgItem(hDlg, IDC_EDIT3);
        Hmatrix[0][2] = GetDlgItem(hDlg, IDC_EDIT4);
        Hmatrix[0][3] = GetDlgItem(hDlg, IDC_EDIT5);
        Hmatrix[0][4] = GetDlgItem(hDlg, IDC_EDIT6);
        Hmatrix[1][0] = GetDlgItem(hDlg, IDC_EDIT33);
        Hmatrix[1][1] = GetDlgItem(hDlg, IDC_EDIT35);
        Hmatrix[1][2] = GetDlgItem(hDlg, IDC_EDIT9);
        Hmatrix[1][3] = GetDlgItem(hDlg, IDC_EDIT10);
        Hmatrix[1][4] = GetDlgItem(hDlg, IDC_EDIT11);
        Hmatrix[2][0] = GetDlgItem(hDlg, IDC_EDIT12);
        Hmatrix[2][1] = GetDlgItem(hDlg, IDC_EDIT36);
        Hmatrix[2][2] = GetDlgItem(hDlg, IDC_EDIT14);
        Hmatrix[2][3] = GetDlgItem(hDlg, IDC_EDIT15);
        Hmatrix[2][4] = GetDlgItem(hDlg, IDC_EDIT16);
        Hmatrix[3][0] = GetDlgItem(hDlg, IDC_EDIT17);
        Hmatrix[3][1] = GetDlgItem(hDlg, IDC_EDIT18);
        Hmatrix[3][2] = GetDlgItem(hDlg, IDC_EDIT19);
        Hmatrix[3][3] = GetDlgItem(hDlg, IDC_EDIT20);
        Hmatrix[3][4] = GetDlgItem(hDlg, IDC_EDIT21);
        Hmatrix[4][0] = GetDlgItem(hDlg, IDC_EDIT22);
        Hmatrix[4][1] = GetDlgItem(hDlg, IDC_EDIT23);
        Hmatrix[4][2] = GetDlgItem(hDlg, IDC_EDIT24);
        Hmatrix[4][3] = GetDlgItem(hDlg, IDC_EDIT25);
        Hmatrix[4][4] = GetDlgItem(hDlg, IDC_EDIT26);

        Hmatrixb[0] = GetDlgItem(hDlg, IDC_EDIT27);
        Hmatrixb[1] = GetDlgItem(hDlg, IDC_EDIT28);
        Hmatrixb[2] = GetDlgItem(hDlg, IDC_EDIT29);
        Hmatrixb[3] = GetDlgItem(hDlg, IDC_EDIT30);
        Hmatrixb[4] = GetDlgItem(hDlg, IDC_EDIT31);

        hList = GetDlgItem(hDlg, IDC_LIST2);

        ChangeInf(hDlg);
    }
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_COMBO2:
        {

            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                ChangeInf(hDlg);
            }
        }
        break;
        case IDC_BUTTON3:
        {

            bool flag = false;
            SendMessageA(hList, LB_RESETCONTENT, 0, 0);
            string S;
            TCHAR gg[100];
            for (int i = 0; i < (cur + 1); i++)
                for (int j = 0; j < (cur + 1); j++) {
                    GetWindowText(Hmatrix[i][j], gg, 100);
                    Amatrix[i][j] = _ttoi(gg);
                }
            for (int i = 0; i < (cur + 1); i++) {
                GetWindowText(Hmatrixb[i], gg, 100);
                Bmatrix[i] = _ttoi(gg);
            }
            for (int i = 0; i < (cur + 1); i++) {
                double sum1 = -MAXINT16;
                double sum2 = -MAXINT16;
                for (int j = 0; j < (cur + 1); j++) {
                    sum1 += Amatrix[i][j];
                    sum2 += Amatrix[j][i];
                }
                if ((sum1 == -MAXINT16) || (sum2 == -MAXINT16)) {
                    MessageBox(hDlg, _T("Строка или столбец с нулями"), _T("Ошибка!"), MB_OK);
                    flag = true;
                }

            }
            double sum1 = -MAXINT16;
            for (int i = 0; i < (cur + 1); i++)
                sum1 += Bmatrix[i];
            if (sum1 == -MAXINT16) {
                MessageBox(hDlg, _T("Ну b одни нули"), _T("Ошибка!"), MB_OK);
                flag = true;
            }
            if (flag)
                break;
            x = gauss(hDlg, Amatrix, Bmatrix, (cur + 1));

                for (int i = 0; i < (cur + 1); i++) {
                    S = to_string(x[i]);
                    SendMessageA(hList, LB_ADDSTRING, 1, (LPARAM)S.c_str());
                }

        }
        break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            return FALSE;
        }
    }
    break;
    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return FALSE;
    }
    return FALSE;
}

void ChangeInf(HWND hd) {

    cur = SendMessage(DlCombo2, CB_GETCURSEL, 0, 0L);
    switch (cur)
    {
    case 0:
        for (int i = 0; i < 1; i++)
            for (int j = 0; j < 1; j++)
                ShowWindow(Hmatrix[i][j], 1);
        for (int i = 1; i < 5; i++)
            for (int j = 0; j < 5; j++)
                ShowWindow(Hmatrix[i][j], 0);
        for (int j = 1; j < 5; j++)
            ShowWindow(Hmatrix[0][j], 0);
    
        for (int i = 0; i < 1; i++) {
            ShowWindow(Hmatrixb[i], 1);
        }
        for (int i = 1; i < 5; i++) {
            ShowWindow(Hmatrixb[i], 0);
        }
        break;
    case 1:
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                ShowWindow(Hmatrix[i][j], 1);
        for (int i = 2; i < 5; i++)
            for (int j = 0; j < 5; j++)
                ShowWindow(Hmatrix[i][j], 0);
        for (int i = 0; i < 2; i++)
            for(int j = 2; j < 5; j++)
                ShowWindow(Hmatrix[i][j], 0);

        for (int i = 0; i < 2; i++) {
            ShowWindow(Hmatrixb[i], 1);
        }
        for (int i = 2; i < 5; i++) {
            ShowWindow(Hmatrixb[i], 0);
        }
        break;


    case 2:
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                ShowWindow(Hmatrix[i][j], 1);
        for (int i = 3; i < 5; i++)
            for (int j = 0; j < 5; j++)
                ShowWindow(Hmatrix[i][j], 0);
        for (int i = 0; i < 3; i++)
            for (int j = 3; j < 5; j++)
                ShowWindow(Hmatrix[i][j], 0);

        for (int i = 0; i < 3; i++) {
            ShowWindow(Hmatrixb[i], 1);
        }
        for (int i = 3; i < 5; i++) {
            ShowWindow(Hmatrixb[i], 0);
        }
        break;
    case 3:
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                ShowWindow(Hmatrix[i][j], 1);
        for (int j = 0; j < 5; j++)
            ShowWindow(Hmatrix[4][j], 0);
        for (int i = 0; i < 4; i++)
                ShowWindow(Hmatrix[i][4], 0);


        for (int i = 0; i < 4; i++) {
            ShowWindow(Hmatrixb[i], 1);
        }
        for (int i = 4; i < 5; i++) {
            ShowWindow(Hmatrixb[i], 0);
        }
        break;

    case 4:
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                ShowWindow(Hmatrix[i][j], 1);

        for (int i = 0; i < 5; i++) {
            ShowWindow(Hmatrixb[i], 1);
        }

        break;
    default:
        break;
    }
    InvalidateRect(hd, NULL, true);
}

double* gauss(HWND hDlg,double a[5][5], double y[5], int n)
{

    double* x, max;
    int k, index;
    const double eps = 0.00001;  // точность
    x = new double[n];
    k = 0;
    while (k < n)
    {
        // Поиск строки с максимальным a[i][k]
        max = abs(a[k][k]);
        index = k;
        for (int i = k + 1; i < n; i++)
        {
            if (abs(a[i][k]) > max)
            {
                max = abs(a[i][k]);
                index = i;
            }
        }
        // Перестановка строк
        if (max < eps)
        {
            // нет ненулевых диагональных элементов
            return 0;
        }
        for (int j = 0; j < n; j++)
        {
            double temp = a[k][j];
            a[k][j] = a[index][j];
            a[index][j] = temp;
        }
        double temp = y[k];
        y[k] = y[index];
        y[index] = temp;
        // Нормализация уравнений
        for (int i = k; i < n; i++)
        {
            double temp = a[i][k];
            if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] / temp;
            y[i] = y[i] / temp;
            if (i == k)  continue; // уравнение не вычитать само из себя
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] - a[k][j];
            y[i] = y[i] - y[k];
        }
        k++;
    }
    // обратная подстановка
    for (k = n - 1; k >= 0; k--)
    {
        x[k] = y[k];
        for (int i = 0; i < k; i++)
            y[i] = y[i] - a[i][k] * x[k];
    }
    return x;
}