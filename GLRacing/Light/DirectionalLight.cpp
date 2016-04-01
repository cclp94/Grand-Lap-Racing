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
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::vec3 lightDir = glm::normalize(direction);
	glm::mat4 proj = glm::ortho<float>(-500, 500, -500, 500, -1000, 1000);
	glm::mat4  view = glm::lookAt(glm::vec3(0.0, 500, -500), glm::vec3(0.0, 0.0, 0.0f), glm::vec3(0.0, 1.0, 0.0));
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
	glUniform3f(s->getUniform("light.direction"), direction.x, direction.y, direction.z);
	glUniform3f(s->getUniform("light.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(s->getUniform("light.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(s->getUniform("light.specular"), specular.x, specular.y, specular.z);
}

