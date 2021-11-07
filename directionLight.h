#pragma once
#include "light.h"
class DirectionLight : public Light
{
public:
	DirectionLight(Vector3d direction);

	virtual float calculateLuminance(Vector3d normal);
};

