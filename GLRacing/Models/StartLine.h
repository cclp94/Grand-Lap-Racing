#pragma once
#include "ImportedModel.h"
class StartLine :
	public ImportedModel
{
public:
	StartLine(Shader *s);
	~StartLine();
	glm::vec4 position;
private:

};

