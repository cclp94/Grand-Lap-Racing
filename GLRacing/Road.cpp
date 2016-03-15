#include "Road.h"
#include "SplineFactory.h"



Road::Road(Shader *s) : Model(s)
{
		color = glm::vec3(0.3, 0.3, 0.3);
		getModel();
		setupMesh();
}


Road::~Road()
{
}

void Road::getModel() {
	float y = -0.001;
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.0);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.5);
	vertices.push_back(-0.2); vertices.push_back(y); vertices.push_back(0.8);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(0.9);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(0.7);
	vertices.push_back(0.1); vertices.push_back(y); vertices.push_back(0.2);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(0.0);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(-0.3);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(-0.5);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(-0.7);
	vertices.push_back(-0.7); vertices.push_back(y); vertices.push_back(-0.3);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(-0.2);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.0);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.5);
	vertices.push_back(-0.2); vertices.push_back(y); vertices.push_back(0.8);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(0.9);

	SplineFactory s;
	vertices = s.buildSpline(vertices);
	vertices = s.extrudeSpline(vertices, 50);

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] *= 500;
	}

}

void Road::draw() {

	glBindVertexArray(VAO);

	glUniform3f(shaderProgram->getUniform("vertex_color"), color.x, color.y, color.z);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() /3);

	glBindVertexArray(0);
}


