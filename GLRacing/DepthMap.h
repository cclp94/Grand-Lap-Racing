#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\ext.hpp>

class DepthMap
{
public:
	DepthMap();
	~DepthMap();
	void bind();
	void unbind();
	GLuint getId();
private:
	const static GLuint SHADOW_WIDTH = 1000, SHADOW_HEIGHT = 1000;
	GLuint FBO, depthMapId;
};

