#include "kart.h"



kart::kart() {
	maxSpeed = 100.0f;
	acceleration = 0.5f;
	speed = 0.0f;
	getModel();
}

float*  kart::getModelVertices() {
	return &vertices[0];
}

GLuint kart::getModelsize() {
	return sizeof(GLfloat) * vertices.size();
}

void kart::accelerate() {
	if (speed == maxSpeed) {
		speed = 100;
	}
	else {
		speed += acceleration;
	}
}

void kart::deaccelerate() {
	if (speed == -maxSpeed) {
		speed = -100;
	}
	else {
		speed -= acceleration;
	}
}

float kart::getSpeed() {
	return speed / 100;
}

void kart::update() {
	if (speed < 0)
		speed += 0.05;
	else if(speed > 0)
		speed -= 0.05;
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