#pragma once
#include "ImportedModel.h"

class Bridge :
	public ImportedModel
{
public:
	Bridge(Shader *s);
	~Bridge();
	void depthDraw(Shader *s);
	void  draw();
	glm::vec4 position;
};

