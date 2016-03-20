#pragma once
#include "ImportedModel.h"
class StartLine :
	public ImportedModel
{
public:
	StartLine(Shader *s);
	~StartLine();
	void depthDraw(Shader *s);
	void  draw();
	glm::vec4 position;
};

