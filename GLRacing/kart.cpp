#include "kart.h"



kart::kart() : Model(){
	color = glm::vec3(1.0, 0.1, 0.2);
	maxSpeed = 100.0f;
	acceleration = 0.2f;
	speed = 0.0f;
	isAccelarating = false;
	getModel();
	setupMesh();
}

void kart::notAccelerating() {
	isAccelarating = false;
}

void kart::accelerating() {
	isAccelarating = true;
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

glm::vec3 kart::getColor() {
	return color;
}

void kart::draw(GLuint color_id) {
	glBindVertexArray(VAO);

	glUniform3f(color_id, color.x, color.y, color.z);

	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void kart::getModel() {

		vertices.push_back(0.5);	vertices.push_back(0.5);	vertices.push_back(0.5);
		vertices.push_back(0.0f);	vertices.push_back(0.5);	vertices.push_back(0.5);
		vertices.push_back(0.5);	vertices.push_back(0.5);	vertices.push_back(0.0f);
		vertices.push_back(0.0f);	vertices.push_back(0.5);	vertices.push_back(0.0f);
		vertices.push_back(0.5);	vertices.push_back(0.0f);	vertices.push_back(0.5);
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(0.5);
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(0.0f);
		vertices.push_back(0.5);	vertices.push_back(0.0f);	vertices.push_back(0.0f);


		indices.push_back(3); indices.push_back(2);indices.push_back(6);
		indices.push_back(7); indices.push_back(4); indices.push_back(2); indices.push_back(0);
		indices.push_back(3); indices.push_back(1);indices.push_back(6);
		indices.push_back(5); indices.push_back(4); indices.push_back(1); indices.push_back(0);



}