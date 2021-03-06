#include "Water.h"



Water::Water(Shader *s) : Model(s){
	getModel();
	height = -5;
	setupMesh();

	//Sets position
	model_matrix = glm::rotate(model_matrix, glm::radians(-10.0f), glm::vec3(0.0, 1.0, 0.0));
	model_matrix = glm::translate(model_matrix, glm::vec3(70.0, height, -300));
	model_matrix = glm::scale(model_matrix, glm::vec3(100.0, 0.0, 230.00));
	
	moveFactor = 0.01f;
	
}

float Water::getMoveFactor() {
	return moveFactor;
}

void Water::draw() {

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Water::depthDraw(Shader *s) {

	glBindVertexArray(VAO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


Water::~Water()
{
	
}

void Water::getModel() {
	float y = -1.0;
	vertices.push_back(-1); vertices.push_back(y); vertices.push_back(-1.0);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(-1.0);
	vertices.push_back(-1); vertices.push_back(y); vertices.push_back(1.0);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(1.0);

	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);
}
