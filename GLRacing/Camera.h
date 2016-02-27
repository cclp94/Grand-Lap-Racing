#pragma once

#include <iostream>
#include <glm\ext.hpp>
#include "kart.h"


class Camera
{
public:
	Camera();
	~Camera();
	

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	friend class kart;
	
};

