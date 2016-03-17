#pragma once

#include <glm/ext.hpp>
#include "Shader.h"

class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 direc, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec);
	~DirectionalLight();
	glm::vec3 getDirection();
	glm::vec3 getAmbientColor();
	glm::vec3 getDiffuseColor();
	glm::vec3 getSpecularColor();
	glm::mat4 getLightViewMatrix();
	glm::mat4 getLightProjectionMatrix();
	glm::mat4 getLightSpaceMatrix(int width, int height);
	void setProperties(Shader *s);

private:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

};

