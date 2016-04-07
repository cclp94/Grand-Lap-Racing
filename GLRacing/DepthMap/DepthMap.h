#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\ext.hpp>

/*
	Render scene from light's perspective and generate a depth texture
*/

class DepthMap
{
public:
	DepthMap(int width, int height);
	~DepthMap();
	void bind(int width, int height);
	void unbind();
	GLuint getId();
private:
	GLuint FBO, depthMapId;
};

