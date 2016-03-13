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
	glm::vec3 color;
	glm::vec3 drawSpline(float u, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2,
		glm::vec3 p3);
	void subdivide(float u0, float u1, float angleThreshold,
		glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	vector<GLfloat> spline;
};

