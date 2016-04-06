#pragma once
#include "Model.h"
#include <glm\vec3.hpp>

class BillboardModel :
	public Model
{
public:
	BillboardModel(Shader *s, glm::vec3 translation, glm::vec3 scale, string textureName, float * angle);
	BillboardModel(Shader *s, glm::vec3 translation, glm::vec3 scale, GLuint textureID, float * angle);
	~BillboardModel();
	void draw();
	void setPosition(glm::vec3 trans, glm::vec3 scale);

private:
	void getModel();
	void getTexture(string textureName);
	float * rotationAngle;
	glm::vec3 translation;
	glm::vec3 scale;
};

