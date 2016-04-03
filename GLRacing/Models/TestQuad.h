#pragma once

#include "Model.h"


class TestQuad : 
	public Model
{
public:
	TestQuad(Shader *s, float xOffset);
	~TestQuad();
	
	void draw();
	void setTexture(int textId);

private:
	void getModel();
};

