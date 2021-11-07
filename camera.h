#pragma once
#include "vector3d.h"

class Camera
{
public:
	Camera();
	Camera(Vector3d position);

	Vector3d getPosition();
	void setPosition(Vector3d newPos);

private:
	Vector3d position;
};

