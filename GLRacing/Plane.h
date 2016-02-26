#pragma once

#include "Model.h"
class Plane :
	public Model
{
public:
	Plane();
	~Plane();
	void  draw(GLuint color_id);
	glm::vec3  getColor();
private:
	void getModel();
	glm::vec3 color;
};

