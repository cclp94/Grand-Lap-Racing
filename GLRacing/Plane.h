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
	void depthDraw(Shader *s);
	float getHeight(float modelX, float ModelZ);
	static const int GRAVITY = -10;
private:
	void getModel();
	glm::vec3 color;
	const static int TERRAIN_WIDTH = 200;
	const static int TERRAIN_DEPTH = 200;
	const static int SCALE = 5;
	const static int offsetX = 500, offsetZ = 500, offsetY = -1;
	glm::vec2 HALF_TERRAIN_SIZE;
	vector<GLfloat> normals;
	GLuint VBO2;
	GLfloat heights[TERRAIN_WIDTH][TERRAIN_DEPTH];
	
	float findHeight(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
};

