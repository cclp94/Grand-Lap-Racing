#include "TestQuad.h"



TestQuad::TestQuad(Shader *s, float xOffset) : Model(s)
{
	getModel();
	setupMesh();

	model_matrix = glm::translate(model_matrix, glm::vec3(xOffset, 0, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(5));

}


TestQuad::~TestQuad()
{
}

void TestQuad::setTexture(int textId) {
	texture = textId;
}

void TestQuad::getModel() {
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

void TestQuad::draw() {
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shaderProgram->getUniform("texture_diffuse1"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
