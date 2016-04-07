#pragma once
#include "ImportedModel.h"

/*
	Windmill model with animation
*/
class Windmill :
	public ImportedModel
{
public:
	Windmill(Shader *s, string objFile,
		glm::vec3 trans, glm::vec3 scale, float rot);
	~Windmill();
	void draw();
	void depthDraw(Shader *s);

private:
	double rotationTime;
};

