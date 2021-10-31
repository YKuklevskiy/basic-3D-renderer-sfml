#include "triangle.h"
#include <exception>

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3) 
	: vertices{ p1, p2, p3 } { }

Triangle::Triangle(Vector3d vertices[3])
	: vertices{ vertices[0], vertices[1], vertices[2] } { }

Vector3d& Triangle::operator[](int i)
{
	if (i < 0 || i > 2)
		throw std::exception("Index out of range");

	return vertices[i];
}

void Triangle::translate(Vector3d t)
{
	for (int i = 0; i < 3; i++)
		vertices[i] = vertices[i] + t;
}

void Triangle::translate(float x, float y, float z)
{
	Vector3d temp(x, y, z);
	for (int i = 0; i < 3; i++)
		vertices[i] = vertices[i] + temp;
}

void Triangle::scale(float s)
{
	for (int i = 0; i < 3; i++)
		vertices[i] = vertices[i] * s;
}

void Triangle::scaleXY(float xScale, float yScale)
{
	for (int i = 0; i < 3; i++)
	{
		vertices[i].x *= xScale;
		vertices[i].y *= yScale;
	}
}
