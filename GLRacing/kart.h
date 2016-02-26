#pragma once

#include <iostream>
#include <vector>

#include "Model.h"

using namespace std;

class kart : public Model{
public:
	kart();
	float getSpeed();
	void update();
	void accelerate();
	void deaccelerate();
	void notAccelerating();
	void accelerating();
	void  draw(GLuint color_id);
	glm::vec3  getColor();
private:
	float maxSpeed;
	float speed;
	float acceleration;
	void getModel();
	bool isAccelarating;
	bool isReverse;
	glm::vec3 color;
};