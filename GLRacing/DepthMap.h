#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\ext.hpp>

class DepthMap
{
public:
	DepthMap(int width, int height);
	~DepthMap();
	void bind(int width, int height);
	void unbind();
	GLuint getId();
	glm::mat4 getBias();
private:
	GLuint FBO, depthMapId;
	GLuint depthrenderbuffer;
	glm::mat4 bias;
};

