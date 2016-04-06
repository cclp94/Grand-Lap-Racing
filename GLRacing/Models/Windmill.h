#pragma once
#include "C:\Users\Caio\Code\C-C++\COMP 371\Project\GLRacing\GLRacing\Models\ImportedModel.h"
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

