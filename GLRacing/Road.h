#pragma once
#include "Model.h"
class Road :
	public Model
{
public:
	Road(Shader *s);
	~Road();
	void draw();
private:
	void getModel();
};

