#pragma once
#include "../Models/Model.h"
class Water :
	public Model
{
public:
	Water(Shader *s);
	~Water();
	void draw();
	void depthDraw(Shader *s);
	float getHeight() {
		return height;
	}
	float getMoveFactor();
	

private:
	void getModel();
	float height;
	float moveFactor;
	
};

