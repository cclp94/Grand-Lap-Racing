#pragma once
#include "Model.h"
class Road :
	public Model
{
public:
	Road(Shader *s);
	~Road();
	void draw();
	void depthDraw(Shader *s);
private:
	void getModel();
};

