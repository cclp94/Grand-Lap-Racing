#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <gl\glew.h>

#include <GLFW\glfw3.h>

using namespace std;

class Shader
{
public:
	Shader(string fileName, int type);
	GLuint getShaderId();
	static GLuint makeProgram(GLuint vShader, GLuint fShader);
	~Shader();
private:
	void getCode(string fileName);
	void compileShader();

	GLuint shaderId;
	string shaderCode;
};

