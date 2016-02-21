#include "kart.h"



kart::kart() {
	maxSpeed = 100.0f;
	acceleration = 0.2f;
	speed = 0.0f;
	isAccelarating = false;
	getModel();
}

void kart::notAccelerating() {
	isAccelarating = false;
}

void kart::accelerating() {
	isAccelarating = true;
}

float*  kart::getModelVertices() {
	return &vertices[0];
}

GLuint kart::getModelsize() {
	return sizeof(GLfloat) * vertices.size();
}

void kart::accelerate() {
	accelerating();
	isReverse = false;
	if (speed == maxSpeed) {
		speed = 100;
	}
	else {
		speed += acceleration;
	}
}

void kart::deaccelerate() {
	accelerating();
	isReverse = true;
	if (speed == -maxSpeed) {
		speed = -100;
	}
	else {
		speed -= acceleration * 2;		// Breaking faster than acceleration
	}
}

float kart::getSpeed() {
	return speed / 100;
}

void kart::update() {
	if (!isAccelarating) {
		if (speed < 0)
			speed += 0.1;
		else if (speed > 0)
			speed -= 0.1;
	}
	else {
		if (!isReverse) {
			accelerate();
		}
		else
			deaccelerate();
	}
}

void kart::getModel() {

	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);
	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);
	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);

	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);

	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);
	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);

	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);

	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);
	vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f);  vertices.push_back(-0.5f); vertices.push_back(-0.5f);

	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(0.5f);
	vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(-0.5f);


}