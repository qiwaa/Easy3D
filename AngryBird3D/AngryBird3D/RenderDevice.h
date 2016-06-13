#ifndef _render_device_h
#define _render_device_h

#include "Math3D.h"
#include "main.h"

class RenderDevice
{
private:
	transform_t transform;      // 坐标变换器
	int width;                  // 窗口宽度
	int height;                 // 窗口高度
	IUINT32 **framebuffer;      // 像素缓存：framebuffer[y] 代表第 y行
	float **zbuffer;            // 深度缓存：zbuffer[y] 为第 y行指针

	//纹理相关
	IUINT32 **texture;          // 纹理：同样是每行索引
	int tex_width;              // 纹理宽度
	int tex_height;             // 纹理高度
	float max_u;                // 纹理最大宽度：tex_width - 1
	float max_v;                // 纹理最大高度：tex_height - 1

	int render_state;           // 渲染状态
	IUINT32 foreground;         // 线框颜色

public:
	void device_init(int width, int height, void *fb);
	void device_pixel(int x, int y, IUINT32 color);
	void device_draw_line(int x1, int y1, int x2, int y2, IUINT32 c);
	//by me----------------------
	transform_t& GetTransform();
	int GetRenderState();
	void SetRenderState(int state);
	void SetForeground(IUINT32 color);
	IUINT32 GetForeground();
	//texture--------------------
	void device_set_texture(void *bits, long pitch, int w, int h);
	void device_draw_scanline(scanline_t *scanline);
	void device_render_trap(trapezoid_t *trap);
	IUINT32 device_texture_read(float u, float v);

};

#endif