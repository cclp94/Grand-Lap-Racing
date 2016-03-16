#include "Plane.h"
#include <SOIL\SOIL.h>



Plane::Plane(Shader *s) : Model(s)
{
	color = glm::vec3(0.2, 0.5, 0.2);
	material.ambient = glm::vec4(color, 1.0);
	material.diffuse = glm::vec4(0.5, 0.6, 0.2, 1.0);
	material.specular = glm::vec4(0.7, 0.5, 0.2, 1.0);
	material.shininess = 5;
	getModel();
	setupMesh();
	HALF_TERRAIN_SIZE = glm::vec2(TERRAIN_WIDTH / 2, TERRAIN_DEPTH / 2);
}


Plane::~Plane()
{
}

glm::vec3 Plane::getColor() {
	return color;
}

void Plane::draw() {
	

	glBindVertexArray(VAO);

	glUniform1i(shaderProgram->getUniform("heightMapTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	

	glUniform1i(shaderProgram->getUniform("normalMapTexture"), 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, NormalMap);


	
	glUniform2i(shaderProgram->getUniform("HALF_TERRAIN_SIZE"), TERRAIN_WIDTH >> 1, TERRAIN_DEPTH >> 1);
	glUniform1f(shaderProgram->getUniform("scale"), 10);
	glUniform1f(shaderProgram->getUniform("half_scale"), 6);

	glUniform3f(shaderProgram->getUniform("vertex_color"), color.x, color.y, color.z);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	setMaterialUniform();
	//glUniformMatrix4fv(shaderProgram->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void Plane::getModel() {

	pData = SOIL_load_image("terrain2.tga", &texture_width, &texture_height, &channels, SOIL_LOAD_L);

	glGenTextures(1, &Texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_width, texture_height, 0, GL_RED, GL_UNSIGNED_BYTE, pData);
	SOIL_free_image_data(pData);


	for (int j = 0; j<TERRAIN_DEPTH; j+=2) {
		for (int i = 0; i<TERRAIN_WIDTH; i+=2) {
			vertices.push_back((float(i) / (TERRAIN_WIDTH - 1)));
			vertices.push_back(-0.0001);
			vertices.push_back(float(j) / (TERRAIN_DEPTH - 1));
		}
	}

	int totalPoints = vertices.size()/3;
	int depth = (TERRAIN_DEPTH / 2);
	int width = (TERRAIN_WIDTH / 2);
	for (int i = 0; i < depth -1; i++)
	{
		for (int j = 0; j < width -1; j++)
		{
			indices.push_back(((depth*i) + j) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
		}
	}
	//getNormal();

	pData = SOIL_load_image("TerrainNormalMap.tga", &texture_width, &texture_height, &channels, SOIL_LOAD_RGB);

	glGenTextures(1, &NormalMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NormalMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_width, texture_height, 0, GL_RED, GL_UNSIGNED_BYTE, pData);
	SOIL_free_image_data(pData);

}

void Plane::setupMesh() {


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	if (!indices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &(indices[0]), GL_STATIC_DRAW);
	}
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);
}
