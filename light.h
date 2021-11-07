#pragma once
#include "vector3d.h"

class Light
{
public:
	Light(Vector3d position, Vector3d direction);

	virtual float calculateLuminance(Vector3d normal) = 0;

protected:
	Vector3d position;
	Vector3d direction;
};

