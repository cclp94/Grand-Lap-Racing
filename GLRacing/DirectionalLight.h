#pragma once

#include <glm/vec3.hpp>

class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 direc, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec);
	~DirectionalLight();
	glm::vec3 getDirection();
	glm::vec3 getAmbientColor();
	glm::vec3 getDiffuseColor();
	glm::vec3 getSpecularColor();


private:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

};

