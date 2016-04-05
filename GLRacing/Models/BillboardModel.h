#pragma once
#include "Model.h"
class BillboardModel :
	public Model
{
public:
	BillboardModel(Shader *s, glm::vec3 translation, glm::vec3 scale, string textureName);
	~BillboardModel();
	void draw();
	void setTurningAngle(float turnAngle);
	void setPosition(glm::vec3 trans, glm::vec3 scale, float turningAngle);

private:
	void getModel();
	void getTexture(string textureName);
};

