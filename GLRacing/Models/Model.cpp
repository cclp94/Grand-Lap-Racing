#include "Model.h"


Model::Model(Shader *shader)
{
	shaderProgram = shader;
}


Model::~Model()
{
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


/*
	set buffers up depending of entered values for models
*/
void Model::setupMesh() {


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &TexVBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	if (!indices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &(indices[0]), GL_STATIC_DRAW);
	}
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	if (!normals.empty()) {

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
	}

	if (!texCoords.empty()) {
		// TexCoord attribute
		glBindBuffer(GL_ARRAY_BUFFER, TexVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	glBindVertexArray(0);
}

void Model::setMaterialUniform() {
	glUniform4f(shaderProgram->getUniform("material.ambient"), material.ambient.x, material.ambient.y, material.ambient.z, material.ambient.w);
	glUniform4f(shaderProgram->getUniform("material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z, material.diffuse.w);
	glUniform4f(shaderProgram->getUniform("material.specular"), material.specular.x, material.specular.y, material.specular.z, material.specular.w);
	glUniform1f(shaderProgram->getUniform("material.shininess"), material.shininess);

}
