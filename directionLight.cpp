#include "directionLight.h"
#include <iostream>

DirectionLight::DirectionLight(Vector3d direction)
	: Light({0, 0, 0}, direction) { }

float DirectionLight::calculateLuminance(Vector3d normal)
{
	return direction / normal;
}