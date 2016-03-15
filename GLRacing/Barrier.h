#pragma once
#include "Model.h"
class Barrier :
	public Model
{
public:
	Barrier(Shader *s, int position);
	~Barrier();
	void draw();
	const static int INNER = 3, OUTTER = 0;
	int position;
private:
	void getModel();
};

