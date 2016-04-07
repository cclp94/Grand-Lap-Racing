#pragma once
#include <iostream>
#include <vector>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\ext.hpp>

#include "../Shaders/Shader.h";
#include "Material.h"

using namespace std;

/*
	Abstract class for Models
*/

class Model {
public:
	Model(Shader *shader);
	virtual ~Model();
	vector <GLfloat> vertices;
	vector <GLuint> indices;
	virtual void draw() = 0;
	virtual glm::vec3 getNormal() { return glm::vec3(0.0, 1.0, 0.0); }
	virtual glm::vec3 getPosition() { return glm::vec3(model_matrix * glm::vec4(0.0, 0.0, 0.0, 1.0)); }

protected:
	GLuint VAO, VBO, EBO, texture;
	vector<GLfloat> normals, texCoords;
	GLuint VBO2, TexVBO;

	Shader *shaderProgram;
	glm::mat4 view_matrix, model_matrix;
	glm::vec3 vertex_color;
	glm::vec3 color;
	int texture_width, texture_height, channels=1;
	unsigned char* pData;
	virtual void setupMesh();
	Material material;
	void setMaterialUniform();
};

