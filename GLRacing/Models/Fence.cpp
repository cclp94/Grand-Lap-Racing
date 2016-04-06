#include "Fence.h"
#include "ImportedModel.h"
#include "../SOIL.h"


Fence::Fence(Shader *s, glm::vec3 trans, float scale) : Model(s)
{
	getModel();
	setupMesh();

	model_matrix = glm::translate(model_matrix, trans);
	model_matrix = glm::scale(model_matrix, glm::vec3(scale, 1, scale));
}


Fence::~Fence()
{
}

void Fence::getModel() {
	unsigned char* pData = SOIL_load_image("Assets/Textures/animals/fence.png", &texture_width, &texture_height, &channels, SOIL_LOAD_RGBA);
	if (pData == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "res_texture.png" << ")" << endl;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	SOIL_free_image_data(pData);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbi


	float y = -1.0;
	
	vertices.push_back(-1.0f); vertices.push_back(0.0); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(0.0); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0); vertices.push_back(1.0f);
	

	vertices.push_back(-1.0f); vertices.push_back(1.0); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(0.0); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(0.0); vertices.push_back(-1.0f);

	vertices.push_back(1.0f); vertices.push_back(0.0); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(0.0); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0); vertices.push_back(1.0f);

	vertices.push_back(-1.0f); vertices.push_back(0.0); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(0.0); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0); vertices.push_back(-1.0f);

	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);
	indices.push_back(4); indices.push_back(5); indices.push_back(6);
	indices.push_back(4); indices.push_back(6); indices.push_back(7);
	indices.push_back(8); indices.push_back(9); indices.push_back(10);
	indices.push_back(8); indices.push_back(10); indices.push_back(11);
	indices.push_back(12); indices.push_back(13); indices.push_back(14);
	indices.push_back(12); indices.push_back(14); indices.push_back(15);		

	texCoords.push_back(6.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(0.0);
	texCoords.push_back(6.0); texCoords.push_back(0.0);

	texCoords.push_back(0.0); texCoords.push_back(0.0);
	texCoords.push_back(6.0); texCoords.push_back(0.0);
	texCoords.push_back(6.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(1.0);

	texCoords.push_back(6.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(0.0);
	texCoords.push_back(6.0); texCoords.push_back(0.0);

	texCoords.push_back(6.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(1.0);
	texCoords.push_back(0.0); texCoords.push_back(0.0);
	texCoords.push_back(6.0); texCoords.push_back(0.0);
	normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);
}



void Fence::draw() {
	
	
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shaderProgram->getUniform("texture_diffuse1"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}