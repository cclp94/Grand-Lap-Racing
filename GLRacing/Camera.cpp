#include "Camera.h"



Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	theta = 0.0;
	phi = 0.0;
}


Camera::~Camera()
{
}
