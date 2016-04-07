#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <gl\glew.h>

#include <GLFW\glfw3.h>

using namespace std;

/*
	Creates shader program from vertex shader and fragment shader files
*/

class Shader
{
public:
	Shader(string vertexFileName, string fragmentFileName);
	GLuint getUniform(const GLchar* name);
	void use();
	void unuse();
	~Shader();
	GLuint programID;
private:
	string getCode(string fileName);
	void checkStatus(GLuint shaderID);
	void makeProgram(GLuint vShader, GLuint fShader);

	
};

