#pragma once
#include "ImportedModel.h"

class Bridge :
	public ImportedModel
{
public:
	Bridge(Shader *s);
	~Bridge();
	glm::vec4 position;
	float getHeight(float modelX, float modelY);

private:
	float collisionRadius;
	glm::vec4 collisionCenter;
};

