#include "Math3D.h"
#include "RenderDevice.h"
#include <math.h>
#include "main.h"

void Math3D::camera_at_zero(RenderDevice *device, float x, float y, float z) {
	point_t eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	//matrix_set_lookat(&device->transform.view, &eye, &at, &up);
	//transform_update(&device->transform);
	return;
}

//һάֵ����--------------------------------------------------------------------------------

//��x��min��max֮��
int Math3D::CMID(int x, int min, int max) { return (x < min)? min : ((x > max)? max : x); }

// �����ֵ��t Ϊ [0, 1] ֮�����ֵ
float Math3D::interp(float x1, float x2, float t) { return x1 + (x2 - x1) * t; }

//ʸ������----------------------------------------------------------------------------------

// | v |
float Math3D::vector_length(const vector_t *v) {
	float sq = v->x * v->x + v->y * v->y + v->z * v->z;
	return (float)sqrt(sq);
}

// z = x + y
void Math3D::vector_add(vector_t *z, const vector_t *x, const vector_t *y) {
	z->x = x->x + y->x;
	z->y = x->y + y->y;
	z->z = x->z + y->z;
	z->w = 1.0;
}

// z = x - y
void Math3D::vector_sub(vector_t *z, const vector_t *x, const vector_t *y) {
	z->x = x->x - y->x;
	z->y = x->y - y->y;
	z->z = x->z - y->z;
	z->w = 1.0;
}

// ʸ�����
float Math3D::vector_dotproduct(const vector_t *x, const vector_t *y) {
	return x->x * y->x + x->y * y->y + x->z * y->z;
}

// ʸ�����	�����ʽ��u x v = { u2v3-v2u3 , u3v1-v3u1 , u1v2-u2v1 }
void Math3D::vector_crossproduct(vector_t *z, const vector_t *x, const vector_t *y) {
	float m1, m2, m3;
	m1 = x->y * y->z - x->z * y->y;
	m2 = x->z * y->x - x->x * y->z;
	m3 = x->x * y->y - x->y * y->x;
	z->x = m1;
	z->y = m2;
	z->z = m3;
	z->w = 1.0f;
}

// ʸ����ֵ��tȡֵ [0, 1]
void Math3D::vector_interp(vector_t *z, const vector_t *x1, const vector_t *x2, float t) {
	z->x = interp(x1->x, x2->x, t);
	z->y = interp(x1->y, x2->y, t);
	z->z = interp(x1->z, x2->z, t);
	z->w = 1.0f;
}

// ʸ����һ��
void Math3D::vector_normalize(vector_t *v) {
	float length = vector_length(v);
	if (length != 0.0f) {
		float inv = 1.0f / length;
		v->x *= inv; 
		v->y *= inv;
		v->z *= inv;
	}
}

//��������----------------------------------------------------------

//a = b
void Math3D::matrix_set_equal(matrix_t *a, const matrix_t *b) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			a->m[i][j] = b->m[i][j];
	}
}

// c = a + b
void Math3D::matrix_add(matrix_t *c, const matrix_t *a, const matrix_t *b) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] + b->m[i][j];
	}
}

// c = a - b
void Math3D::matrix_sub(matrix_t *c, const matrix_t *a, const matrix_t *b) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] - b->m[i][j];
	}
}

// c = a * b
void Math3D::matrix_mul(matrix_t *c, const matrix_t *a, const matrix_t *b) {
	matrix_t z;
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			z.m[j][i] = (a->m[j][0] * b->m[0][i]) +
				(a->m[j][1] * b->m[1][i]) +
				(a->m[j][2] * b->m[2][i]) +
				(a->m[j][3] * b->m[3][i]);
		}
	}
	c[0] = z;
}

// c = a * f
void Math3D::matrix_scale(matrix_t *c, const matrix_t *a, float f) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) 
			c->m[i][j] = a->m[i][j] * f;
	}
}

// y = x * m
void Math3D::matrix_apply(vector_t *y, const vector_t *x, const matrix_t *m) {
	float X = x->x, Y = x->y, Z = x->z, W = x->w;
	y->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
	y->y = X * m->m[0][1] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
	y->z = X * m->m[0][2] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
	y->w = X * m->m[0][3] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
}

void Math3D::matrix_set_identity(matrix_t *m) {
	m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f; 
	m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}

void Math3D::matrix_set_zero(matrix_t *m) {
	m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
}

// ƽ�Ʊ任
void Math3D::matrix_set_translate(matrix_t *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}

// ���ű任
void Math3D::matrix_set_scale(matrix_t *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[0][0] = x;
	m->m[1][1] = y;
	m->m[2][2] = z;
}

// ���������
void Math3D::matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *at, const vector_t *up) {
	vector_t xaxis, yaxis, zaxis;

	vector_sub(&zaxis, at, eye);
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis);
	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);

	m->m[0][0] = xaxis.x;
	m->m[1][0] = xaxis.y;
	m->m[2][0] = xaxis.z;
	m->m[3][0] = -vector_dotproduct(&xaxis, eye);	// -eye*xaxis 

	m->m[0][1] = yaxis.x;
	m->m[1][1] = yaxis.y;
	m->m[2][1] = yaxis.z;
	m->m[3][1] = -vector_dotproduct(&yaxis, eye);

	m->m[0][2] = zaxis.x;
	m->m[1][2] = zaxis.y;
	m->m[2][2] = zaxis.z;
	m->m[3][2] = -vector_dotproduct(&zaxis, eye);

	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
}

// D3DXMatrixPerspectiveFovLH  �Ƕȡ����߱ȡ���ƽ����롢Զƽ�����
void Math3D::matrix_set_perspective(matrix_t *m, float fovy, float aspect, float zn, float zf) {
	float cot = 1.0f / (float)tan(fovy * 0.5f);
	matrix_set_zero(m);
	m->m[0][0] = (float)(cot / aspect);
	m->m[1][1] = (float)(cot);
	m->m[2][2] = zf / (zf - zn);
	m->m[3][2] = - zn * zf / (zf - zn);
	m->m[2][3] = 1;
}

// ������£����� transform = world * view * projection
void Math3D::transform_update(transform_t *ts) {
	matrix_t m;
	matrix_mul(&m, &ts->world, &ts->view);
	matrix_mul(&ts->transform, &m, &ts->projection);
}

// ��ʼ����������Ļ����
void Math3D::transform_init(transform_t *ts, int width, int height) {
	float aspect = (float)width / ((float)height);
	matrix_set_identity(&ts->world);
	matrix_set_identity(&ts->view);
	matrix_set_perspective(&ts->projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
	ts->w = (float)width;
	ts->h = (float)height;
	transform_update(ts);
}

//vertex--------------------------------------------------------

//��ʼ����������ʾ��ص�����,����u/z,v/z,1/z
void Math3D::vertex_rhw_init(vertex_t *v) {
	float rhw = 1.0f / v->pos.w;		// 1/��������Z	�� 1/z
	v->rhw = rhw;
	v->tc.u *= rhw;						// u/z,����֮�����1/z�����u
	v->tc.v *= rhw;
	v->color.r *= rhw;
	v->color.g *= rhw;
	v->color.b *= rhw;
}

void Math3D::vertex_interp(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float t) {
	y->rhw = interp(x1->rhw, x2->rhw, t);
	vector_interp(&y->pos, &x1->pos, &x2->pos, t);
	y->tc.u = interp(x1->tc.u, x2->tc.u, t);
	y->tc.v = interp(x1->tc.v, x2->tc.v, t);
	y->color.r = interp(x1->color.r, x2->color.r, t);
	y->color.g = interp(x1->color.g, x2->color.g, t);
	y->color.b = interp(x1->color.b, x2->color.b, t);
}

//y���������ÿ�ƶ�һ�����أ���������
void Math3D::vertex_division(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float w) {
	float inv = 1.0f / w;
	y->pos.x = (x2->pos.x - x1->pos.x) * inv;
	y->pos.y = (x2->pos.y - x1->pos.y) * inv;
	y->pos.z = (x2->pos.z - x1->pos.z) * inv;
	y->pos.w = (x2->pos.w - x1->pos.w) * inv;
	y->tc.u = (x2->tc.u - x1->tc.u) * inv;
	y->tc.v = (x2->tc.v - x1->tc.v) * inv;
	y->color.r = (x2->color.r - x1->color.r) * inv;
	y->color.g = (x2->color.g - x1->color.g) * inv;
	y->color.b = (x2->color.b - x1->color.b) * inv;
	y->rhw = (x2->rhw - x1->rhw) * inv;
}

void Math3D::vertex_add(vertex_t *y, const vertex_t *x) {
	y->pos.x += x->pos.x;
	y->pos.y += x->pos.y;
	y->pos.z += x->pos.z;
	y->pos.w += x->pos.w;
	y->rhw += x->rhw;
	y->tc.u += x->tc.u;
	y->tc.v += x->tc.v;
	y->color.r += x->color.r;
	y->color.g += x->color.g;
	y->color.b += x->color.b;
}

// �������������� 0-2 �������Σ����ҷ��غϷ����ε�����
int Math3D::trapezoid_init_triangle(trapezoid_t *trap, const vertex_t *p1, 
	const vertex_t *p2, const vertex_t *p3) {
		const vertex_t *p;
		float k, x;

		if (p1->pos.y > p2->pos.y) p = p1, p1 = p2, p2 = p;
		if (p1->pos.y > p3->pos.y) p = p1, p1 = p3, p3 = p;
		if (p2->pos.y > p3->pos.y) p = p2, p2 = p3, p3 = p;
		if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;			//3����һ��ֱ����
		if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

		if (p1->pos.y == p2->pos.y) {			// triangle down����ΪWindows��y�������Զ�����������
			if (p1->pos.x > p2->pos.x) p = p1, p1 = p2, p2 = p;
			trap[0].top = p1->pos.y;			//������y��Сֵ
			trap[0].bottom = p3->pos.y;			//������y���ֵ
			trap[0].left.v1 = *p1;				//��������ߵ�yֵ��С�ĵ�
			trap[0].left.v2 = *p3;				//��������ߵ�yֵ���ĵ�
			trap[0].right.v1 = *p2;				//�ұ������ߵ�yֵ��С�ĵ�
			trap[0].right.v2 = *p3;				//�ұ������ߵ�yֵ���ĵ�
			return (trap[0].top < trap[0].bottom)? 1 : 0;		//����1
		}

		if (p2->pos.y == p3->pos.y) {	// triangle up
			if (p2->pos.x > p3->pos.x) p = p2, p2 = p3, p3 = p;
			trap[0].top = p1->pos.y;
			trap[0].bottom = p3->pos.y;
			trap[0].left.v1 = *p1;
			trap[0].left.v2 = *p2;
			trap[0].right.v1 = *p1;
			trap[0].right.v2 = *p3;
			return (trap[0].top < trap[0].bottom)? 1 : 0;
		}

		//���Ϊ��������������
		trap[0].top = p1->pos.y;
		trap[0].bottom = p2->pos.y;
		trap[1].top = p2->pos.y;
		trap[1].bottom = p3->pos.y;

		k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
		x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

		if (x <= p3->pos.x) {		// triangle left��p2��p1p3�߶ε����
			trap[0].left.v1 = *p1;
			trap[0].left.v2 = *p2;
			trap[0].right.v1 = *p1;
			trap[0].right.v2 = *p3;
			trap[1].left.v1 = *p2;
			trap[1].left.v2 = *p3;
			trap[1].right.v1 = *p1;
			trap[1].right.v2 = *p3;
		}	else {					// triangle right��p2��p1p3�߶ε��ұ�
			trap[0].left.v1 = *p1;
			trap[0].left.v2 = *p3;
			trap[0].right.v1 = *p1;
			trap[0].right.v2 = *p2;
			trap[1].left.v1 = *p1;
			trap[1].left.v2 = *p3;
			trap[1].right.v1 = *p2;
			trap[1].right.v2 = *p3;		//����ȥ�ĵ���p1��p2��p3����ϣ�֮���ٽ��о����ֵ
		}

		return 2;
}

// ���� Y ��������������������������� Y �Ķ��㣬����ֵ�õ�����ʱ�������.v
void Math3D::trapezoid_edge_interp(trapezoid_t *trap, float y) {
	float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
	float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
	float t1 = (y - trap->left.v1.pos.y) / s1;
	float t2 = (y - trap->right.v1.pos.y) / s2;
	vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
	vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
}

// �����������ߵĶ˵㣬��ʼ�������ɨ���ߵ����Ͳ���
void Math3D::trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y) {
	float width = trap->right.v.pos.x - trap->left.v.pos.x;
	scanline->x = (int)(trap->left.v.pos.x + 0.5f);
	scanline->w = (int)(trap->right.v.pos.x + 0.5f) - scanline->x;
	scanline->y = y;
	scanline->v = trap->left.v;
	if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
	vertex_division(&scanline->step, &trap->left.v, &trap->right.v, width);
}