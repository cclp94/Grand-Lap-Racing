#pragma once
#include "model.h"
#include "SOIL.h"

using namespace std;

class skybox : public Model
{
public:
	skybox(Shader *s);
	~skybox();
	void  draw();
	void depthDraw(Shader *s);

private:
	void getModel();
	GLuint loadCubemap(vector<const GLchar*> faces);	
};