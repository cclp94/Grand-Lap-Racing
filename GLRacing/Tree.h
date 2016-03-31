#pragma once
#include "Model.h"
class Tree :
	public Model
{
public:
	Tree(Shader *s);
	~Tree();
	void draw(float turnAngle);
	void draw();

private:
	void getModel();
};

