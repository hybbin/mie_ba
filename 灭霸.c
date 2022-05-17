#ifdef _MSC_VER
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
#include <stdio.h>　　
#include <Windows.h>
#include <winbase.h>　
#include<conio.h>
#define MONITOR_ON -1
#define MONITOR_OFF (LPARAM)2
#define MONITOR_STANBY 1
#define IDR_PAUSE 12
#define IDR_ABOUT 13
#define IDR_EXIT 15
#define IDR_NOW5 17
LPCTSTR szAppClassName = TEXT("灭霸1.0");
LPCTSTR szAppWindowName = TEXT("灭霸1.0");
HMENU hmenu;//菜单句柄
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NOTIFYICONDATA nid;
    UINT WM_TASKBARCREATED;
    POINT pt;//用于接收鼠标坐标
    int xx;//用于接收菜单选项返回值
    // 不要修改TaskbarCreated，这是系统任务栏自定义的消息
    WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
    switch (message)
    {
    case WM_CREATE://窗口创建时候的消息.
        nid.cbSize = sizeof(nid);
        nid.hWnd = hwnd;
        nid.uID = 0;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_USER;
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        lstrcpy(nid.szTip, szAppClassName);
        Shell_NotifyIcon(NIM_ADD, &nid);
        hmenu = CreatePopupMenu();//生成菜单
        AppendMenu(hmenu, MF_STRING, IDR_PAUSE, L"设置");//为菜单添加两个选项
        AppendMenu(hmenu, MF_STRING, IDR_NOW5, L"5秒后熄屏");//为菜单添加两个选项
        AppendMenu(hmenu, MF_STRING, IDR_ABOUT, L"关于");
        AppendMenu(hmenu, MF_STRING, IDR_EXIT, L"退出");
        break;
    case WM_USER://连续使用该程序时候的消息.
        if (lParam == WM_LBUTTONDOWN)
            //MessageBox(hwnd, TEXT("Win32 API 实现系统托盘程序,双击托盘可以退出!"), szAppClassName, MB_OK);
            if (lParam == WM_LBUTTONDBLCLK)
                //复位
                SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, lParam);
        //ShowWindow(hwnd,SW_SHOWMINNOACTIVE);
        //双击托盘的消息,退出.
        //SendMessage(hwnd, WM_CLOSE, wParam, lParam);
        if (lParam == WM_RBUTTONDOWN)
        {
            GetCursorPos(&pt);//取鼠标坐标
            SetForegroundWindow(hwnd);//解决在菜单外单击左键菜单不消失的问题
            //EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED);//让菜单中的某一项变灰
            EnableMenuItem(hmenu, IDR_PAUSE, MF_GRAYED);
            xx = TrackPopupMenu(hmenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL);//显示菜单并获取选项ID
            if (xx == IDR_PAUSE) ShowWindow(hwnd, SW_SHOW);
            if (xx == IDR_ABOUT) MessageBox(hwnd, TEXT("周一至周五8:30后自动亮屏；\n每天18:30至第二天8:30之前，无人操作电脑后2分钟自动熄屏，有人操作电脑2分钟内不会熄屏。\n--hybin"), szAppClassName, MB_OK);
            if (xx == IDR_EXIT) SendMessage(hwnd, WM_CLOSE, wParam, lParam);
            if (xx == IDR_NOW5) { Sleep(5000); PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, MONITOR_OFF); }
            if (xx == 0) PostMessage(hwnd, WM_LBUTTONDOWN, NULL, NULL);
            //MessageBox(hwnd, TEXT("右键"), szAppName, MB_OK);
        }
        break;
    case WM_DESTROY://窗口销毁时候的消息.
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;
    default:
        /*
        * 防止当Explorer.exe 崩溃以后，程序在系统系统托盘中的图标就消失
        *
        * 原理：Explorer.exe 重新载入后会重建系统任务栏。当系统任务栏建立的时候会向系统内所有
        * 注册接收TaskbarCreated 消息的顶级窗口发送一条消息，我们只需要捕捉这个消息，并重建系
        * 统托盘的图标即可。
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
                    printf("用户离开超过2分钟，立即熄屏\n");
                    xp();
                }
                else
                {
                    printf("用户离开未超过2分钟\n");
                }
            }
            if (TimeNub >= 830.0 && TimeNub < 900.0)
            {
                if (WorkDay == 1 || WorkDay == 2 || WorkDay == 3 || WorkDay == 4 || WorkDay == 5)
                {
                    printf("上班时间到，该亮屏了\n");
                    lp();
                }
                else
                {
                    printf("今天是休息日，不用上班\n");
                }
            }
        }
        Sleep(2000);
    }
}


int main(int argc, char* argv[])
{
    //ShowWindow(FindWindow("ConsoleWindowClass", argv[0]), 0);
    //添加自启动
    ziqi();
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    HWND handle = FindWindow(NULL, szAppWindowName);
    if (handle != NULL)
    {
        MessageBox(NULL, TEXT("已经有一个实例在运行了！在任务托盘里面。"), szAppClassName, MB_ICONERROR);
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
    // 此处使用WS_EX_TOOLWINDOW 属性来隐藏显示在任务栏上的窗口程序按钮
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

//获取当前程序文件目录
//int lujing()
//{
//    char strModule[256];
//    GetModuleFileNameA(NULL, strModule, 256);//得到当前模块路径
//    char strWork[1000];
//    int i = 1000;
//    GetCurrentDirectoryA(1000, strWork);//得到当前工作路径
//    printf("当前模块路径: %s \n工作路径：%s \n", strModule, strWork);
//    getchar();
//    return strModule;
//}

//添加注册表开机自启动
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
        ret = RegSetValueEx(hkey, TEXT("亮熄屏"), 0, REG_SZ, (LPBYTE)szModule,(lstrlen(szModule)+1)*sizeof(TCHAR));
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