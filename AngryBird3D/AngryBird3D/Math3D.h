#ifndef _math_3d_h
#define _math_3d_h

#include <cstdio>
#include "RenderDevice.h"
#include "vector4.h"

class Math3D
{
public:
	static void camera_at_zero(RenderDevice *device, float x, float y, float z);

    static int CMID(int x, int min, int max);
	static float vector_length(const vector_t *v);
	static float interp(float x1, float x2, float t);
	static void vector_add(vector_t *z, const vector_t *x, const vector_t *y);
	static void vector_sub(vector_t *z, const vector_t *x, const vector_t *y);
	static float vector_dotproduct(const vector_t *x, const vector_t *y);
	static void vector_crossproduct(vector_t *z, const vector_t *x, const vector_t *y);
    static void vector_interp(vector_t *z, const vector_t *x1, const vector_t *x2, float t);
	static void vector_normalize(vector_t *v);
	static void matrix_set_equal(matrix_t *a, const matrix_t *b);
	static void matrix_add(matrix_t *c, const matrix_t *a, const matrix_t *b);
	static void matrix_sub(matrix_t *c, const matrix_t *a, const matrix_t *b);
	static void matrix_mul(matrix_t *c, const matrix_t *a, const matrix_t *b);
	static void matrix_scale(matrix_t *c, const matrix_t *a, float f);
	static void matrix_apply(vector_t *y, const vector_t *x, const matrix_t *m);
	static void matrix_set_identity(matrix_t *m);
	static void matrix_set_zero(matrix_t *m);
	static void matrix_set_translate(matrix_t *m, float x, float y, float z);
	static void matrix_set_scale(matrix_t *m, float x, float y, float z);
	static void matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *at, const vector_t *up);
 	static void matrix_set_perspective(matrix_t *m, float fovy, float aspect, float zn, float zf) ;
	static void transform_update(transform_t *ts);
	static void transform_init(transform_t *ts, int width, int height);

	static void vertex_rhw_init(vertex_t *v);
	static void vertex_interp(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float t);
	static void vertex_division(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float w);
	static void vertex_add(vertex_t *y, const vertex_t *x);
	static int trapezoid_init_triangle(trapezoid_t *trap, const vertex_t *p1, 
		const vertex_t *p2, const vertex_t *p3);
	static void trapezoid_edge_interp(trapezoid_t *trap, float y);
	static void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y);
};

#endif