#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\vec2.hpp>

struct Character {
	GLuint textureID;  
	glm::ivec2 Size;       
	glm::ivec2 Bearing;    
	GLuint Advance;    
};