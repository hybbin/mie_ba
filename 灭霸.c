#ifdef _MSC_VER
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
#include <stdio.h>����
#include <Windows.h>
#include <winbase.h>��
#include<conio.h>
#define MONITOR_ON -1
#define MONITOR_OFF (LPARAM)2
#define MONITOR_STANBY 1
#define IDR_PAUSE 12
#define IDR_ABOUT 13
#define IDR_EXIT 15
#define IDR_NOW5 17
LPCTSTR szAppClassName = TEXT("���1.0");
LPCTSTR szAppWindowName = TEXT("���1.0");
HMENU hmenu;//�˵����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NOTIFYICONDATA nid;
    UINT WM_TASKBARCREATED;
    POINT pt;//���ڽ����������
    int xx;//���ڽ��ղ˵�ѡ���ֵ
    // ��Ҫ�޸�TaskbarCreated������ϵͳ�������Զ������Ϣ
    WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
    switch (message)
    {
    case WM_CREATE://���ڴ���ʱ�����Ϣ.
        nid.cbSize = sizeof(nid);
        nid.hWnd = hwnd;
        nid.uID = 0;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_USER;
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        lstrcpy(nid.szTip, szAppClassName);
        Shell_NotifyIcon(NIM_ADD, &nid);
        hmenu = CreatePopupMenu();//���ɲ˵�
        AppendMenu(hmenu, MF_STRING, IDR_PAUSE, L"����");//Ϊ�˵��������ѡ��
        AppendMenu(hmenu, MF_STRING, IDR_NOW5, L"5���Ϩ��");//Ϊ�˵��������ѡ��
        AppendMenu(hmenu, MF_STRING, IDR_ABOUT, L"����");
        AppendMenu(hmenu, MF_STRING, IDR_EXIT, L"�˳�");
        break;
    case WM_USER://����ʹ�øó���ʱ�����Ϣ.
        if (lParam == WM_LBUTTONDOWN)
            //MessageBox(hwnd, TEXT("Win32 API ʵ��ϵͳ���̳���,˫�����̿����˳�!"), szAppClassName, MB_OK);
            if (lParam == WM_LBUTTONDBLCLK)
                //��λ
                SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, lParam);
        //ShowWindow(hwnd,SW_SHOWMINNOACTIVE);
        //˫�����̵���Ϣ,�˳�.
        //SendMessage(hwnd, WM_CLOSE, wParam, lParam);
        if (lParam == WM_RBUTTONDOWN)
        {
            GetCursorPos(&pt);//ȡ�������
            SetForegroundWindow(hwnd);//����ڲ˵��ⵥ������˵�����ʧ������
            //EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED);//�ò˵��е�ĳһ����
            EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED);
            xx = TrackPopupMenu(hmenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL);//��ʾ�˵�����ȡѡ��ID
            if (xx == IDR_PAUSE) ShowWindow(hwnd, SW_SHOW);
            if (xx == IDR_ABOUT) MessageBox(hwnd, TEXT("��һ������8:30���Զ�������\nÿ��18:30���ڶ���8:30֮ǰ�����˲������Ժ�2�����Զ�Ϩ�������˲�������2�����ڲ���Ϩ����\n--hybin"), szAppClassName, MB_OK);
            if (xx == IDR_EXIT) SendMessage(hwnd, WM_CLOSE, wParam, lParam);
            if (xx == IDR_NOW5) { Sleep(5000); PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, MONITOR_OFF); }
            if (xx == 0) PostMessage(hwnd, WM_LBUTTONDOWN, NULL, NULL);
            //MessageBox(hwnd, TEXT("�Ҽ�"), szAppName, MB_OK);
        }
        break;
    case WM_DESTROY://��������ʱ�����Ϣ.
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;
    default:
        /*
        * ��ֹ��Explorer.exe �����Ժ󣬳�����ϵͳϵͳ�����е�ͼ�����ʧ
        *
        * ԭ��Explorer.exe �����������ؽ�ϵͳ����������ϵͳ������������ʱ�����ϵͳ������
        * ע�����TaskbarCreated ��Ϣ�Ķ������ڷ���һ����Ϣ������ֻ��Ҫ��׽�����Ϣ�����ؽ�ϵ
        * ͳ���̵�ͼ�꼴�ɡ�
        */
        if (message == WM_TASKBARCREATED)
            SendMessage(hwnd, WM_CREATE, wParam, lParam);
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
int zhu()
{
    SYSTEMTIME time;
    SYSTEMTIME clock;
    LASTINPUTINFO lpi;
    lpi.cbSize = sizeof(lpi);
    while (1)
    {
        BOOL bVal = GetLastInputInfo(&lpi);
        if (bVal)
        {
            GetLocalTime(&time);
            float TimeNub = time.wHour * 100 + time.wMinute;
            printf("%f", TimeNub);
            int WorkDay = time.wDayOfWeek;
            if (TimeNub >= 1830.0 && TimeNub < 830.0)
            {
                UINT gap = (GetTickCount() - lpi.dwTime) / 1000;
                if (gap > 120)
                {
                    printf("�û��뿪����2���ӣ�����Ϩ��\n");
                    xp();
                }
                else
                {
                    printf("�û��뿪δ����2����\n");
                }
            }
            if (TimeNub >= 830.0 && TimeNub < 900.0)
            {
                if (WorkDay == 1 || WorkDay == 2 || WorkDay == 3 || WorkDay == 4 || WorkDay == 5)
                {
                    printf("�ϰ�ʱ�䵽����������\n");
                    lp();
                }
                else
                {
                    printf("��������Ϣ�գ������ϰ�\n");
                }
            }
        }
        Sleep(2000);
    }
}


int main(int argc, char* argv[])
{
    //ShowWindow(FindWindow("ConsoleWindowClass", argv[0]), 0);
    //���������
    ziqi();
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    HWND handle = FindWindow(NULL, szAppWindowName);
    if (handle != NULL)
    {
        MessageBox(NULL, TEXT("�Ѿ���һ��ʵ���������ˣ��������������档"), szAppClassName, MB_ICONERROR);
        return 0;
    }
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = NULL;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppClassName;
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppClassName, MB_ICONERROR);
        return 0;
    }
    // �˴�ʹ��WS_EX_TOOLWINDOW ������������ʾ���������ϵĴ��ڳ���ť
    hwnd = CreateWindowEx(WS_EX_TOOLWINDOW,
        szAppClassName, szAppWindowName,
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, NULL, NULL);
    ShowWindow(hwnd, 0);
    UpdateWindow(hwnd);
    CreateThread(NULL, 0, zhu, NULL, 0, NULL);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;

}

//��ȡ��ǰ�����ļ�Ŀ¼
//int lujing()
//{
//    char strModule[256];
//    GetModuleFileNameA(NULL, strModule, 256);//�õ���ǰģ��·��
//    char strWork[1000];
//    int i = 1000;
//    GetCurrentDirectoryA(1000, strWork);//�õ���ǰ����·��
//    printf("��ǰģ��·��: %s \n����·����%s \n", strModule, strWork);
//    getchar();
//    return strModule;
//}

//���ע�����������
int ziqi()
{
    //char s[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

    HKEY hkey;
    DWORD ret;
    ret = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hkey);
    if (ret == ERROR_SUCCESS)
    {
        TCHAR szModule[MAX_PATH];
        GetModuleFileName(NULL,szModule,MAX_PATH);
        ret = RegSetValueEx(hkey, TEXT("��Ϩ��"), 0, REG_SZ, (LPBYTE)szModule,(lstrlen(szModule)+1)*sizeof(TCHAR));
    }
    RegCloseKey(hkey);
}

int xp()
{
    PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, MONITOR_OFF);
    return 0;
}
int lp()
{
    PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, MONITOR_ON);
    ht();
    return 0;
}
int ht()
{
    POINT lpPoint;
    GetCursorPos(&lpPoint);
    int mx = lpPoint.x * 65535 / GetSystemMetrics(SM_CXSCREEN);
    int my = lpPoint.y * 65535 / GetSystemMetrics(SM_CYSCREEN);
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, mx, my, 0, 0);
}