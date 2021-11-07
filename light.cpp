#include "light.h"

Light::Light(Vector3d position, Vector3d direction)
	: position(position), direction(direction) 
{
	direction.normalize();
};