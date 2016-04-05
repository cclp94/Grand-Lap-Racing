#include "BillboardModel.h"
#include "../SOIL.h"


BillboardModel::BillboardModel(Shader *s, glm::vec3 translation, glm::vec3 scale, string textureName) : Model(s)
{
	getModel();
	setupMesh();
	
	//model_matrix = glm::translate(model_matrix, glm::vec3(-220.0, -1, 0.0));
	//model_matrix = glm::scale(model_matrix, glm::vec3(20));

	model_matrix = glm::translate(model_matrix, translation);
	model_matrix = glm::scale(model_matrix, scale);

	getTexture(textureName);
}

void BillboardModel::getTexture(string textureName) {
	string file = "Assets/Textures/" + textureName;
	unsigned char* pData = SOIL_load_image(file.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_RGBA);
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
}


BillboardModel::~BillboardModel()
{
}

void BillboardModel::setPosition(glm::vec3 trans, glm::vec3 scale, float turningAngle) {
	model_matrix = glm::translate(glm::mat4(), trans);
	model_matrix = glm::scale(model_matrix, scale);
	model_matrix = glm::rotate(model_matrix, turningAngle, glm::vec3(0.0, 1.0, 0.0));
}

void BillboardModel::getModel() {
	


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

void BillboardModel::draw() {

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shaderProgram->getUniform("texture_diffuse1"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}