#pragma once
#include <iostream>
#include <vector>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\ext.hpp>

#include "Shader.h";

using namespace std;

class Model {
public:
	virtual float* getModelVertices();
	virtual  GLuint getModelsize();
	Model(Shader *shader);
	virtual ~Model();
	vector <GLfloat> vertices;
	vector <GLuint> indices;
	virtual void draw() = 0;

protected:
	GLuint VAO, VBO, EBO, Texture;

	Shader *shaderProgram;
	glm::mat4 view_matrix, model_matrix;
	glm::vec3 vertex_color;
	glm::vec3 color;
	int texture_width, texture_height, channels;
	GLubyte* pData;
	virtual void setupMesh();

};

