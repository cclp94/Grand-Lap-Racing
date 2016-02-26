#pragma once
#include <iostream>
#include <vector>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\ext.hpp>

using namespace std;

class Model {
public:
	virtual float* getModelVertices();
	virtual  GLuint getModelsize();
	Model();
	virtual ~Model();
	vector <GLfloat> vertices;
	vector <GLuint> indices;
	virtual void draw(GLuint color_id) = 0;

protected:
	GLuint VAO, VBO, EBO;
	virtual void setupMesh();

};

