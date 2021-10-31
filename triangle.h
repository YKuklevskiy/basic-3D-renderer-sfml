#pragma once
#include "vector3d.h"
struct Triangle
{
	Triangle(Vector3d p1, Vector3d p2, Vector3d p3);
	Triangle(Vector3d vertices[3]);

	Vector3d& operator[](int i);
	void translate(Vector3d t);
	void translate(float x, float y, float z);
	void scale(float s);
	void scaleXY(float xScale, float yScale);

	Vector3d vertices[3];
};

