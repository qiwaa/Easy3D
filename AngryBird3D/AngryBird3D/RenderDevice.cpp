#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include "Math3D.h"
#include "RenderDevice.h"
#include "main.h"

//设备初始化、释放-------------------------------------------------------------

// 设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
void RenderDevice::device_init(int width, int height, void *fb)
{
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 4;
	char *ptr = (char*)malloc(need);

	char *framebuf,*zbuf;

	this->width = width;
	this->height = height;

	//framebuffer
	assert(ptr);	//断言
	this->framebuffer = (IUINT32**)ptr;		//height个void*空间
	if (fb != NULL) framebuf = (char*)fb;		//让framebuf指向位图
	for (int i = 0; i < height; i++) {
	this->framebuffer[i] = (IUINT32*)(framebuf + width * 4 * i);	//让指针指向framebuf指向的那片空间
	}
	ptr += sizeof(void*) * height;

	//指向zbuff区域的指针 
	this->zbuffer = (float**)ptr;		//height个void*空间
	ptr += sizeof(void*) * height;

	//zbuff区域 
	zbuf = (char*)ptr;		//width * height * 4个字节空间
	ptr += width * height * 4;
	for (int i = 0; i < height; i++) {
		this->zbuffer[i] = (float*)(zbuf + width * 4 * i);	//让指针指向framebuf指向的那片空间
	}
	for (int j= 0 ; j<height; j++)
	{
		for (int i=0 ; i<width ; i++)
		{
			this->zbuffer[j][i]=1;
		}
	}

	this->texture = (IUINT32**)ptr;			//1024个void*空间
	ptr += sizeof(void*) * 1024;

	Math3D::transform_init(&this->transform, width, height);

	
}

//画点---------------------------------------

// 画点
void RenderDevice::device_pixel(int x, int y, IUINT32 color) {
	if (((IUINT32)x) < (IUINT32)this->width && ((IUINT32)y) < (IUINT32)this->height) {
		this->framebuffer[y][x] = color;
	}
}

// 绘制线段
void RenderDevice::device_draw_line(int x1, int y1, int x2, int y2, IUINT32 c) {
	int x, y, rem = 0;
	if (x1 == x2 && y1 == y2) {			//一个点
		device_pixel(x1, y1, c);
	}	else if (x1 == x2) {			//竖线
		int inc = (y1 <= y2)? 1 : -1;
		for (y = y1; y != y2; y += inc) device_pixel(x1, y, c);
		device_pixel(x2, y2, c);
	}	else if (y1 == y2) {			//横线
		int inc = (x1 <= x2)? 1 : -1;
		for (x = x1; x != x2; x += inc) device_pixel(x, y1, c);
		device_pixel(x2, y2, c);
	}	else {
		int dx = (x1 < x2)? x2 - x1 : x1 - x2;
		int dy = (y1 < y2)? y2 - y1 : y1 - y2;
		if (dx >= dy) {				//宽比高长
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;	//交换顶点，使x1<x2
			for (x = x1, y = y1; x <= x2; x++) {
				device_pixel(x, y, c);
				rem += dy;
				if (rem >= dx) {			//累积rem，每次+dy，当rem>=dx时，多画一个像素，rem-dx
					rem -= dx;
					y += (y2 >= y1)? 1 : -1;
					device_pixel(x, y, c);
				}
			}
			device_pixel(x2, y2, c);
		}	else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				device_pixel(x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1)? 1 : -1;
					device_pixel(x, y, c);
				}
			}
			device_pixel(x2, y2, c);
		}
	}
}

// 设置当前纹理
void RenderDevice::device_set_texture(void *bits, long pitch, int w, int h) {
	char *ptr = (char*)bits;
	assert(w <= 1024 && h <= 1024);
	for (int j = 0; j < h; ptr += pitch, j++) 	// 重新计算每行纹理的指针
		this->texture[j] = (IUINT32*)ptr;
	this->tex_width = w;
	this->tex_height = h;
	this->max_u = (float)(w - 1);
	this->max_v = (float)(h - 1);
}

// 主渲染函数
void RenderDevice::device_render_trap(trapezoid_t *trap) {
	scanline_t scanline;
	int top, bottom;

	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	for (int j = top; j < bottom; j++) {

		if (j >= 0 && j < this->height) {
			Math3D::trapezoid_edge_interp(trap, (float)j + 0.5f);

			Math3D::trapezoid_init_scan_line(trap, &scanline, j);

			device_draw_scanline(&scanline);
		}
		if (j >= this->height) break;
	}
}

// 绘制扫描线
void RenderDevice::device_draw_scanline(scanline_t *scanline) {
	int i = scanline->x;
	int lineWidth = scanline->w;
	int screenWidth = this->width;
	int render_state = this->render_state;

	for (; lineWidth > 0; i++, lineWidth--) {
		if (i >= 0 && i <= screenWidth) {
			//float rhw = scanline->v.rhw;
			if (scanline->v.pos.z<this->zbuffer[scanline->y][i]){		//因为两个三角形公用同一条边，深度值相同，会导致看到的边的颜色不确定
				this->zbuffer[scanline->y][i] = scanline->v.pos.z;
				
				if (render_state & RENDER_STATE_COLOR) {
					float r = scanline->v.color.r / scanline->v.rhw;
					float g = scanline->v.color.g / scanline->v.rhw;
					float b = scanline->v.color.b / scanline->v.rhw;
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = Math3D::CMID(R, 0, 255);
					G = Math3D::CMID(G, 0, 255);
					B = Math3D::CMID(B, 0, 255);
					device_pixel(i,scanline->y,(R << 16) | (G << 8) | (B));
				}
				
				if (render_state & RENDER_STATE_TEXTURE) {
					float u = scanline->v.tc.u / scanline->v.rhw;	//通过（u/z）/(1/z)来获取u
					float v = scanline->v.tc.v / scanline->v.rhw;
					IUINT32 cc = this->device_texture_read(u, v);
					device_pixel(i,scanline->y,cc);
				}
			}
		}

		Math3D::vertex_add(&scanline->v, &scanline->step);		//每递增一个像素，线性修改顶点内的数据
		if (i >= screenWidth) break;
	}
}

// 根据坐标读取纹理,四舍五入获得纹理值，不通过插值
IUINT32 RenderDevice::device_texture_read(float u, float v) {
	int x, y;
	u = u * this->max_u;
	v = v * this->max_v;
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = Math3D::CMID(x, 0, this->tex_width - 1);
	y = Math3D::CMID(y, 0, this->tex_height - 1);
	return this->texture[y][x];
}

//get and set------------------------------------------------------

transform_t& RenderDevice::GetTransform()
{
	return transform;
}

int RenderDevice::GetRenderState()
{
	return render_state;
}

void RenderDevice::SetRenderState(int state)
{
	render_state = state;
}

void RenderDevice::SetForeground(IUINT32 color)
{
	foreground = color;
}

IUINT32 RenderDevice::GetForeground()
{
	return foreground;
}