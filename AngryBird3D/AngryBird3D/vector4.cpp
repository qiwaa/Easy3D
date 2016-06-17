#include "vector4.h"

vector4::vector4(float x,float y,float z,float w):x(x),y(y),z(z),w(w)
{
	
}

vector4::vector4(const vector_t& v):x(v.x),y(v.y),z(v.z),w(v.w)
{
		
}

vector4::vector4(const vector4& v):x(v.x),y(v.y),z(v.z),w(v.w)
{
	
}

vector_t vector4::get_vector_t()
{
	vector_t v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

vector4::~vector4()
{
	
}

vector4& vector4::operator+=(const vector4& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w = 1.0f;
	return static_cast<vector4&>(*this);
}
//more effective C++ 22
const vector4 vector4::operator+(const vector4& v)
{
	return vector4(*this) += v;
}

vector4& vector4::operator-=(const vector4& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w = 1.0f;
	return static_cast<vector4&>(*this);
}

const vector4 vector4::operator-(const vector4& v)
{
	return vector4(*this) -= v;
}

float vector4::operator[](int i)
{
	if (i>3||i<0)
	{
		printf("error:");
	}
	switch (i)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	case 3:
		return w;
		break;
	}
}

float vector4::Length()
{
	float sq = x * x + y * y + z * z;
	return (float)sqrt(sq);
}

float dot(const vector4& v1,const vector4& v2)
{
	return	v1.x * v2.x + v1.y * v2.y + v1.z + v2.z;
}