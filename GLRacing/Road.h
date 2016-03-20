#pragma once
#include "Model.h"
#include "Plane.h"
class Road :
	public Model
{
public:
	Road(Shader *s, Plane *terrain);
	~Road();
	void draw();
	void depthDraw(Shader *s);
private:
	void getModel(Plane *terrain);
};

