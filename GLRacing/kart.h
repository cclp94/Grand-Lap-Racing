#pragma once

#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "ImportedModel.h"


#include "Model.h"
class Camera;

using namespace std;

class kart : public ImportedModel{
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
	glm::vec3 getCameraPosition();
	void move();
	void turn(float angle);
	void depthDraw(Shader *s);
private:
	float maxSpeed;
	float speed;
	float acceleration;
	bool isAccelarating;
	bool isReverse;
	glm::vec3 color;
	Camera *camera;
	glm::vec4 position;
};