#include "camera.h"

Camera::Camera() : Camera::Camera({0, 0, 0}) { }

Camera::Camera(Vector3d position) 
    : position(position) { }

Vector3d Camera::getPosition()
{
    return position;
}

void Camera::setPosition(Vector3d newPos)
{
    this->position = position;
}
