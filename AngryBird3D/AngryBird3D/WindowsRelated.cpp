#include "WindowsRelated.h"
#include "main.h"
#include "Math3D.h"

/*
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
long screen_pitch = 0;
*/

WindowsRelated::WindowsRelated():screen_handle(NULL),screen_dc(NULL),screen_hb(NULL),screen_ob(NULL),screen_fb(NULL)
{
	
}

int WindowsRelated::screen_init(int w,int h,TCHAR * title)
{
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, //结构体大小、宽、高、1、图像位数、0、w*h*字节数
		w * h * 4, 0, 0, 0, 0 }  };
	RECT rect = { 0, 0, w, h };	//存储左上右下
	int wx, wy, sx, sy;			//界面宽高、界面左上角位置
	HDC hDC;				//临时变量，上下文环境，包含有关某个设备的绘制属性信息的 Windows 数据结构。所有绘制调用都通过设备上下文对象进行，这些对象封装了用于绘制线条、形状和文本的 Windows API。
	LPVOID ptr;

	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,//窗口的客户区域以“字符边界”对齐，当系统调整窗口的水平位置时，客户区域的左边坐标是8的整数倍
		NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };//类名
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//设置WNDCLASS的熟悉
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	screen_handle = CreateWindow(_T("SCREEN3.1415926"), title,	//产生一个层叠的窗口，有一个标题栏和一个边框|有标题栏|带有系统菜单|有最小化
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return -2;

	hDC = GetDC(screen_handle);				//获取的HDC直接与相关设备沟通
	screen_dc = CreateCompatibleDC(hDC);	//创建的DC，则是与内存中的一个表面相关联
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);//&ptr:该变量接收一个指向DIB位数据值的指针,修改数据会变换windows画面
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);		//返回值为旧的句柄，用于在设备描述表中选择回原先的数据
	screen_fb = (unsigned char*)ptr;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));//左上角、宽、高、清楚客户区所有内容|维持当前Z序|显示窗口
	SetForegroundWindow(screen_handle);//将指定窗口设置到前台，并激活
	ShowWindow(screen_handle, SW_NORMAL);

	memset(screen_fb, -100000, w * h * 4);

	return 0;
}

LRESULT WindowsRelated::screen_events(HWND hWnd, UINT msg, 
	WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_CLOSE: screen_exit = 1; break;
		//case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;		//监听键盘事件
		//case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
		default: return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
}

//读取出所有Windows消息并发出
void WindowsRelated::screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void WindowsRelated::screen_update(void) {
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	screen_dispatch();
}

void* WindowsRelated::getScreenPtr()
{
	return (void *)screen_fb;
}