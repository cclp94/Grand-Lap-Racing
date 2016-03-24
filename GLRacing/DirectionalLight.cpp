#include "DirectionalLight.h"
#include <glm\ext.hpp>


DirectionalLight::DirectionalLight(glm::vec3 direc, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) :
	direction(direc), ambient(amb), diffuse(dif), specular(spec)
{
}


DirectionalLight::~DirectionalLight()
{
}

glm::mat4 DirectionalLight::getLightSpaceMatrix(int width, int height) {
	glm::mat4 view = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);
	glm::mat4 proj = glm::lookAt(direction, glm::vec3(250.0, 0.0, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	return proj * view;
}

glm::vec3 DirectionalLight::getDirection() {
	return direction;

}
glm::vec3 DirectionalLight::getAmbientColor() {
	return ambient;
}
glm::vec3 DirectionalLight::getDiffuseColor() {
	return diffuse;
}
glm::vec3 DirectionalLight::getSpecularColor() {
	return specular;
}

void DirectionalLight::setProperties(Shader *s) {
	s->use();
	glUniform4f(s->getUniform("light.direction"), direction.x, direction.y, direction.z, 1.0);
	glUniform4f(s->getUniform("light.ambient"), ambient.x, ambient.y, ambient.z, 1.0);
	glUniform4f(s->getUniform("light.diffuse"), diffuse.x, diffuse.y, diffuse.z, 1.0);
	glUniform4f(s->getUniform("light.specular"), specular.x, specular.y, specular.z, 1.0);
}

