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
	position = model_matrix * glm::vec4(0.0 ,0.0, 0.0, 1.0);
	collisionRadius = 279.0f;
	collisionCenter = model_matrix * glm::vec4(0, -275.28, 0.91, 1.0);

	//cout << position.x << " " << position.y << " " << position.z << endl;
	
}

float Bridge::getHeight(float modelX, float modelZ) {
	if (modelX > collisionCenter.x + collisionRadius || modelX < collisionCenter.x - collisionRadius ||
		modelZ > collisionCenter.z + collisionRadius || modelZ < collisionCenter.z - collisionRadius) {

		return -99999;
	}
	float x = collisionCenter.x - modelX;
	float z = collisionCenter.z - modelZ;
	float y2 = (collisionRadius*collisionRadius) - (x * x) - (z*z);
	float y = sqrt(y2) + collisionCenter.y;
	return (!isnan(y))? y : -99999;

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



