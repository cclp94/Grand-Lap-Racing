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
#include "Plane.h"
#include "Bridge.h"
#include "Road.h"

#include "Model.h"
#include <irrKlang\irrKlang.h>

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
	glm::mat4 getCameraView();
	glm::vec3 getCameraPosition();
	glm::vec3 getPosition();
	void move(Plane *terrain, Bridge *b, Road *r);
	void turn(float angle);
	void depthDraw(Shader *s);
	glm::mat4 getCameraSky();
	void setCollision(bool col);
	float* getTurnAngle();
	void resetTurnAngle();
	bool isAccelarating;
	glm::vec3 getCurrentPosition();
	void resetGame();
private:
	float maxSpeed;
	float speed;
	float acceleration;
	
	bool isReverse;
	Camera *camera;
	glm::vec4 position;
	float currentHeight;
	bool collision;
	float turnAngle;
	irrklang::ISoundEngine* carSounds;
	irrklang::ISound* accelerationSound;
};