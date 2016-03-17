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
	const static int TERRAIN_WIDTH = 400;
	const static int TERRAIN_DEPTH = 400;
	glm::vec2 HALF_TERRAIN_SIZE;
	vector<GLfloat> normals;
	GLuint VBO2;
};

