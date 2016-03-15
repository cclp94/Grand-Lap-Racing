#include "Barrier.h"
#include "SplineFactory.h"



Barrier::Barrier(Shader *s, int position) : Model(s)
{
	this->position = position;
	color = glm::vec3(1.0, 1.0, 0.2);
	getModel();
	setupMesh();
}


Barrier::~Barrier()
{
}

void Barrier::getModel() {
	float y = -0.0001;
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
	vertices = s.extrudeSpline(vertices, 40);
	vector <GLfloat> mesh;
	for (int i = position, size = vertices.size(); i < size; i += 6)
	{
		mesh.push_back(vertices[i] * 500);
		mesh.push_back(vertices[i + 1] * 500);
		mesh.push_back(vertices[i + 2] * 500);

	}
	
	for (int i = 0, size = mesh.size(); i < size; i+=3) {

		glm::vec4 p = glm::vec4(mesh[i], mesh[i + 1], mesh[i + 2], 1.0);
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.0, 0.5, 0.0));
		p = trans * p;
		mesh.push_back(p.x); mesh.push_back(p.y); mesh.push_back(p.z);
	}
	
	vertices = mesh;

	int totalPoints = vertices.size() / 3;
	int depth = (totalPoints / 2);
	int width = (totalPoints / 2);
	for (int i = 0; i < depth - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			indices.push_back(((depth*i) + j) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
		}
	}

}

void Barrier::draw() {

	glBindVertexArray(VAO);

	glUniform3f(shaderProgram->getUniform("vertex_color"), color.x, color.y, color.z);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	//glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
