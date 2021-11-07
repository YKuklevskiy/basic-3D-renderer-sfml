#pragma once
#include "vector3d.h"
struct Triangle
{
	//
	//	Constructors
	//
	Triangle(Vector3d p1, Vector3d p2, Vector3d p3);
	Triangle(Vector3d vertices[3]);

	// Returns a vertex of index i 
	Vector3d& operator[](int i);
	
	// Translate vector in space
	void translate(Vector3d t);
	void translate(float x, float y, float z = 0.0f);
	
	// Scale vector by a multiplier of s
	void scale(float s);
	
	// Scale only X and Y coordinates, used for already projected vectors usually
	void scaleXY(float xScale, float yScale);
	
private:
	Vector3d vertices[3];
};

