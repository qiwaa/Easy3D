#ifndef _main_h
#define _main_h

#include <string>
#include <iostream>
#include "WindowsRelated.h"
#include "Application.h"

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色

extern int screen_w, screen_h;					//屏幕宽高
extern int screen_exit;					//控制屏幕退出

typedef unsigned int IUINT32;

typedef struct { float m[4][4];
} matrix_t;
typedef struct { float x, y, z, w; } vector_t;
typedef vector_t point_t;

typedef struct { float r, g, b; } color_t;
typedef struct { float u, v; } texcoord_t;
typedef struct { point_t pos; texcoord_t tc; color_t color; float rhw; } vertex_t;

typedef struct { vertex_t v, v1, v2; } edge_t;	//v1:上面的点，v2：下面的点，v：插值的点，临时使用
typedef struct { float top, bottom; edge_t left, right; } trapezoid_t;	//y最小值，y最大值，左边的边，右边的边
typedef struct { vertex_t v, step; int x, y, w; } scanline_t;	//第y行,开始位置x，长度w, 最初为最左边的顶点v，之后递增

typedef struct { 
	matrix_t world;         // 世界坐标变换
	matrix_t view;          // 摄影机坐标变换
	matrix_t projection;    // 投影变换
	matrix_t transform;     // transform = world * view * projection
	float w, h;             // 屏幕大小
}	transform_t;

using namespace std;

#endif