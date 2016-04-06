#include "Windmill.h"



Windmill::Windmill(Shader *s, string objFile,
	glm::vec3 trans, glm::vec3 scale, float rot) : ImportedModel(s, objFile,trans, scale, rot)
{
	rotationTime = 0.5;
}


Windmill::~Windmill()
{
}

void Windmill::draw() {
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	//this->setMaterialUniform();
	glm::mat4 savedModelMatrix = model_matrix;
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		if (i ==0) {
			model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 7.4, 0.0));
			model_matrix = glm::rotate(model_matrix, (GLfloat)(glfwGetTime() * rotationTime), glm::vec3(0.0, 0.0, 1.0));
			model_matrix = glm::translate(model_matrix, glm::vec3(0.0, -7.4, 0.0));
			glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
		}
		else {
			model_matrix = savedModelMatrix;
			glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

		}
		this->meshes[i].Draw(shaderProgram);
	}
}
void Windmill::depthDraw(Shader *s) {
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(s);
}
