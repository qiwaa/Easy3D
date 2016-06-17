#ifndef vector4_h
#define vector4_h

#include "main.h"
#include <cmath>

class vector4
{
public:
	vector4(const vector_t& v);
	vector4(float x = 0.0f,float y = 0.0f,float z = 0.0f,float w = 1.0f);
	vector4(const vector4& v);
	~vector4();

	vector4& operator+=(const vector4& v);
	vector4& operator-=(const vector4& v);

	const vector4 operator+(const vector4& v);
	const vector4 operator-(const vector4& v);
	float operator[](int i);			//不能逐个赋值，1、会出错，2、与游戏引擎一致

	//normal function
	vector_t get_vector_t();
	float Length();

	//static-----------
	static float dot(const vector4& v1,const vector4& v2);

	float x,y,z,w;
};

#endif