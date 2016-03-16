#include "DirectionalLight.h"



DirectionalLight::DirectionalLight(glm::vec3 direc, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) :
	direction(direc), ambient(amb), diffuse(dif), specular(spec)
{
}


DirectionalLight::~DirectionalLight()
{
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

