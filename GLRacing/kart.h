#pragma once

#include <iostream>
#include <vector>
#include <GLFW\glfw3.h>

using namespace std;

class kart {
public:
	kart();
	float getSpeed();
	void update();
	void accelerate();
	void deaccelerate();
	float* getModelVertices();
	GLuint getModelsize();
	void notAccelerating();
	void accelerating();
private:
	float maxSpeed;
	float speed;
	vector <GLfloat> vertices;
	float acceleration;
	void getModel();
	bool isAccelarating;
	bool isReverse;
};