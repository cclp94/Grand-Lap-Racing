#pragma once
#include "Model.h"
class Water :
	public Model
{
public:
	Water(Shader *s);
	~Water();
	void draw();
	void depthDraw(Shader *s);

private:
	void getModel();
};

