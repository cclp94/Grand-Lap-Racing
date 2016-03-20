#include "Bridge.h"



Bridge::Bridge(Shader *s) : ImportedModel(s)
{
	material.ambient = glm::vec4(1, 1, 1, 1.0);
	material.diffuse = glm::vec4(1, 1, 1, 1.0);
	material.specular = glm::vec4(1, 1, 1, 1.0);
	material.shininess = 76.8;

	getModel("Assets/bridge/bridge.obj ");
	

	model_matrix = glm::translate(model_matrix, glm::vec3(100.0, 3.2, -250.0));
	//model_matrix = glm::scale(model_matrix, glm::vec3(0.2, 0.2, 0.2));
	position = model_matrix * glm::vec4();
	
}


Bridge::~Bridge()
{
}

void Bridge::draw() {
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	//this->setMaterialUniform();
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}
void Bridge::depthDraw(Shader *s) {
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}

