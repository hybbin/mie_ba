#ifdef _MSC_VER
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
#include <stdio.h>����
#include <Windows.h>
#include <winbase.h>��
#define MONITOR_ON -1
#define MONITOR_OFF 2
#define MONITOR_STANBY 1
#define IDR_PAUSE 12
#define IDR_ABOUT 13
#define IDR_DELEY 14
#define IDR_EXIT 15
#define IDR_HIDE 16
#define IDR_NOW 17
LPCTSTR szAppClassName = TEXT("��Ϩ��");
LPCTSTR szAppWindowName = TEXT("��Ϩ��beta0.1");
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
        AppendMenu(hmenu, MF_STRING, IDR_HIDE, L"����");//Ϊ�˵��������ѡ��
        AppendMenu(hmenu, MF_STRING, IDR_NOW, L"����Ϩ��");//Ϊ�˵��������ѡ��
        AppendMenu(hmenu, MF_STRING, IDR_DELEY, L"�ӳ�һ��СʱϢ��");//Ϊ�˵��������ѡ��
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
            EnableMenuItem(hmenu, IDR_HIDE, MF_GRAYED);
            EnableMenuItem(hmenu, IDR_DELEY, MF_GRAYED);
            xx = TrackPopupMenu(hmenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL);//��ʾ�˵�����ȡѡ��ID
            if (xx == IDR_PAUSE) ShowWindow(hwnd, SW_SHOW);
            if (xx == IDR_ABOUT) MessageBox(hwnd, TEXT("��һ���������ϰ˵������������ߵ�����--hybin"), szAppClassName, MB_OK);
            if (xx == IDR_EXIT) SendMessage(hwnd, WM_CLOSE, wParam, lParam);
            if (xx == IDR_HIDE) ShowWindow(hwnd, SW_HIDE);
            if (xx == IDR_NOW) PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, MONITOR_OFF);
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
    while (1)
    {
        SYSTEMTIME time;
        SYSTEMTIME clock;
        GetLocalTime(&time);
        Sleep(30000);
        GetLocalTime(&time);
        printf("����������%2d��ʱ��ָ��%2d\n", time.wDayOfWeek, time.wHour);
        if (time.wDayOfWeek == 1 || time.wDayOfWeek == 2 || time.wDayOfWeek == 3 || time.wDayOfWeek == 4 || time.wDayOfWeek == 5)
        {
            if (time.wHour == 8)
            {
                printf("����");
                printf("����ʱ��Ϊ:%2d\n", time.wHour);
                lp();
                //Sleep(30000);
            }
            if (time.wHour == 19)
            {
                printf("Ϣ��");
                printf("Ϣ��ʱ��Ϊ:%2d\n", time.wHour);
                xp();
                //Sleep(30000);
            }
        }

    }
}


int main(int argc, char* argv[])
{
    //ShowWindow(FindWindow("ConsoleWindowClass", argv[0]), 0);
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


//���ע�����������
int ziqi()
{
    char s[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    HKEY hkey;
    RegOpenKey(HKEY_CURRENT_USER, s, &hkey);
    char strModule[256];
    GetModuleFileName(NULL, strModule, 256);
    RegSetValueEx(hkey, "��Ϩ��", 0, REG_SZ, "strModule", 250);
}

int  xp()
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