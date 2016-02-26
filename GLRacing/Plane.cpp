#include "Plane.h"



Plane::Plane()
{
	color = glm::vec3(0.2, 0.5, 0.2);
	getModel();
	setupMesh();
}


Plane::~Plane()
{
}

glm::vec3 Plane::getColor() {
	return color;
}

void Plane::draw(GLuint color_id) {
	glBindVertexArray(VAO);

	glUniform3f(color_id, color.x, color.y, color.z);

	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Plane::getModel() {

	vertices.push_back(11.5);	vertices.push_back(-0.01);	vertices.push_back(100.5);
	vertices.push_back(-11.0f);	vertices.push_back(-0.01);	vertices.push_back(-100.5);
	vertices.push_back(11.5);	vertices.push_back(-0.01);	vertices.push_back(100.0f);
	vertices.push_back(-11.0f);	vertices.push_back(-0.01);	vertices.push_back(100.0f);
	vertices.push_back(11.5);	vertices.push_back(-0.03f);	vertices.push_back(-100.5);
	vertices.push_back(-11.0f);	vertices.push_back(-0.03f);	vertices.push_back(-100.5);
	vertices.push_back(-11.0f);	vertices.push_back(-0.03f);	vertices.push_back(100.0f);
	vertices.push_back(11.5);	vertices.push_back(-0.03f);	vertices.push_back(100.0f);


	indices.push_back(3); indices.push_back(2); indices.push_back(6);
	indices.push_back(7); indices.push_back(4); indices.push_back(2); indices.push_back(0);
	indices.push_back(3); indices.push_back(1); indices.push_back(6);
	indices.push_back(5); indices.push_back(4); indices.push_back(1); indices.push_back(0);



}
