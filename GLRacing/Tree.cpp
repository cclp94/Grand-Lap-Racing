#include "Tree.h"
#include "SOIL.h"


Tree::Tree(Shader *s) : Model(s)
{
	getModel();
	setupMesh();
	
	model_matrix = glm::translate(model_matrix, glm::vec3(-220.0, -1, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(20));
}


Tree::~Tree()
{
}

void Tree::getModel() {
	unsigned char* pData = SOIL_load_image("tree.png", &texture_width, &texture_height, &channels, SOIL_LOAD_RGBA);
	if (pData == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "res_texture.png" << ")" << endl;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	SOIL_free_image_data(pData);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbi


	float y = -1.0;
	vertices.push_back(0); vertices.push_back(0.0); vertices.push_back(0);
	vertices.push_back(1); vertices.push_back(0.0); vertices.push_back(0);
	vertices.push_back(1); vertices.push_back(1.0); vertices.push_back(0);
	vertices.push_back(0); vertices.push_back(1.0); vertices.push_back(0);

	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	texCoords.push_back(1.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(0.0);
	texCoords.push_back(1.0); texCoords.push_back(0.0);

	normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);
}


void Tree::draw(float turnAngle) {

	model_matrix = glm::rotate(model_matrix, turnAngle, glm::vec3(0.0, 1.0, 0.0));

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shaderProgram->getUniform("diffuseTexture"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Tree::draw() {

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shaderProgram->getUniform("diffuseTexture"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}