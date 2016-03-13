#pragma once

#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

#include "Model.h"
class Camera;

using namespace std;

class kart : public Model{
public:
	kart(Shader *s);
	~kart();
	float getSpeed();
	void update();
	void accelerate();
	void deaccelerate();
	void notAccelerating();
	void accelerating();
	void  draw();
	glm::vec3  getColor();
	glm::mat4 getCameraView();
	void move();
	void turn(float angle);
private:
	float maxSpeed;
	float speed;
	float acceleration;
	void getModel();
	bool isAccelarating;
	bool isReverse;
	glm::vec3 color;
	Camera *camera;
	glm::vec4 position;
};