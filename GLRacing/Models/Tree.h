#pragma once
#include "Model.h"
class Tree :
	public Model
{
public:
	Tree(Shader *s);
	~Tree();
	void draw();
	void Tree::setTurningAngle(float turnAngle);

private:
	void getModel();
};

