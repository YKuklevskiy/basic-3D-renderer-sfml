#pragma once
#include "matrix.h"
struct Vector3d
{
	float x, y, z;
	Vector3d(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	
	Vector3d operator +(Vector3d v);
	Vector3d operator -(Vector3d v);
	
	// Dividing by constant
	Vector3d operator /(float s); 
	
	// Multiplying by constant
	Vector3d operator *(float s); 
	
	// Dot Product
	float operator /(Vector3d v); 
	
	// Cross Product
	Vector3d operator *(Vector3d v);

	// Multiplication by matrix
	Vector3d operator *(Matrix m);
	
	// Normalizes vector, returns copy of normalized vector
	Vector3d normalize();

	// Prints vector in the console
	void printVector();
};

