#include "vector3d.h"
#include "amfunctions.h"
#include <iostream>

Vector3d::Vector3d(float x, float y, float z) 
	: x(x), y(y), z(z) { }

Vector3d Vector3d::operator+(Vector3d v)
{
	float x = this->x + v.x;
	float y = this->y + v.y;
	float z = this->z + v.z;
	return Vector3d(x, y, z);
}

Vector3d Vector3d::operator-(Vector3d v)
{
	float x = this->x - v.x;
	float y = this->y - v.y;
	float z = this->z - v.z;
	return Vector3d(x, y, z);
}

Vector3d Vector3d::operator/(float s)
{
	s = 1 / s;
	return Vector3d(x * s, y * s, z * s);
}

Vector3d Vector3d::operator*(float s)
{
	return Vector3d(x * s, y * s, z * s);
}

float Vector3d::operator/(Vector3d v)
{
	return x*v.x + y*v.y + z*v.z;
}

Vector3d Vector3d::operator*(Vector3d v)
{
	float x = this->y * v.z - this->z * v.y;
	float y = this->z * v.x - this->x * v.z;
	float z = this->x * v.y - this->y * v.x;
	return Vector3d(x, y, z);
}

Vector3d Vector3d::operator*(Matrix m)
{
	std::vector<float> t = {x, y, z};
	Matrix res = Matrix(t) * m;
	return Vector3d(res[0][0], res[0][1], res[0][2]);
}

Vector3d Vector3d::normalize()
{
	float invsqrt = Q_rsqrt(x * x + y * y + z * z); // 1/sqrt(x^2+y^2+z^2)
	x *= invsqrt;
	y *= invsqrt;
	z *= invsqrt;
	return Vector3d(*this);
}

void Vector3d::printVector()
{
	std::cout << '(' << x << ", " << y << ", " << z << ')' << std::endl;
}
