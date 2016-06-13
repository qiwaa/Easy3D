#ifndef _windows_related_h
#define _windows_related_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>

class WindowsRelated
{
public:
	WindowsRelated();
	int screen_init(int w,int h,TCHAR * title);
	static LRESULT screen_events(HWND hWnd, UINT msg, 
		WPARAM wParam, LPARAM lParam);
	void screen_dispatch(void);
	void screen_update(void);

	//by me------------------
	void* getScreenPtr();
protected:
private:
		HWND screen_handle;		// 主窗口 HWND
		HDC screen_dc;			// 配套的 HDC
		HBITMAP screen_hb;		// DIB     句柄是一个标识符，拿来标识对象或者项目
		HBITMAP screen_ob;		// 老的 BITMAP
		unsigned char *screen_fb;		// frame buffer,修改里面内容就会修改Windows画面
};

#endif