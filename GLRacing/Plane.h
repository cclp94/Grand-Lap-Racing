#pragma once

#include <glm\vec2.hpp>

#include "Model.h"
class Plane :
	public Model
{
public:
	Plane(Shader *s);
	~Plane();
	void  draw();
	glm::vec3  getColor();
	
private:
	void getModel();
	glm::vec3 color;
	void setupMesh();
	const static int TERRAIN_WIDTH = 1000;
	const static int TERRAIN_DEPTH = 1000;
	glm::vec2 HALF_TERRAIN_SIZE;
};

