#include "kart.h"
#include "Camera.h"


kart::~kart() {
	delete camera;
}



kart::kart() : Model(){
	color = glm::vec3(1.0, 0.1, 0.2);
	position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	maxSpeed = 3.0f;
	acceleration = 0.03f;
	speed = 0.0f;
	isAccelarating = false;
	getModel();
	setupMesh();
	camera = new Camera();
	camera->cameraPos = glm::vec3(position.x, position.y + 1, position.z + 3.0);
}

glm::mat4 kart::move(glm::mat4 model_matrix) {
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, -getSpeed()));
	glm::vec4 previousPos = position;
	position = model_matrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::mat4 trans = glm::translate(glm::mat4(),
		glm::vec3(position.x - previousPos.x, position.y - previousPos.y, position.z - previousPos.z));
		camera->cameraPos = glm::vec3(trans * glm::vec4(camera->cameraPos, 1.0));
		camera->cameraPos = camera->cameraPos + glm::vec3(3 * glm::sin(camera->theta)/2, 0, 0);
		camera->theta = 0;

	return model_matrix;
}

glm::mat4 kart::turn(float angle, glm::mat4 model_matrix) {
	if (speed > 0) {
		model_matrix = glm::rotate(model_matrix, angle, glm::vec3(0.0, 1.0, 0.0));
		position = model_matrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
		camera->theta = angle;
	}
	return model_matrix;
}

glm::mat4 kart::getCameraView() {
	glm::mat4 view_matrix = glm::lookAt(camera->cameraPos,glm::vec3(position),camera->cameraUp);
	return view_matrix;
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
		speed = maxSpeed;
	}
	else {
		speed += acceleration;
	}
}

void kart::deaccelerate() {
	accelerating();
	isReverse = true;
	if (speed == -maxSpeed) {
		speed = -maxSpeed;
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
		if (speed < 0) {
			speed += 0.1;
		}
		else if (speed > 0) {
			speed -= 0.1;
		}
		if (speed < 0.1 && speed > -0.1)
			speed = 0;
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
		vertices.push_back(-0.5f);	vertices.push_back(0.5);	vertices.push_back(0.5);
		vertices.push_back(0.3);	vertices.push_back(0.3);	vertices.push_back(-0.5f);
		vertices.push_back(-0.3f);	vertices.push_back(0.3);	vertices.push_back(-0.5f);
		vertices.push_back(0.5);	vertices.push_back(0.0f);	vertices.push_back(0.5);
		vertices.push_back(-0.5f);	vertices.push_back(0.0f);	vertices.push_back(0.5);
		vertices.push_back(-0.3f);	vertices.push_back(0.1f);	vertices.push_back(-0.5f);
		vertices.push_back(0.3);	vertices.push_back(0.1f);	vertices.push_back(-0.5f);


		indices.push_back(3); indices.push_back(2);indices.push_back(6);
		indices.push_back(7); indices.push_back(4); indices.push_back(2); indices.push_back(0);
		indices.push_back(3); indices.push_back(1);indices.push_back(6);
		indices.push_back(5); indices.push_back(4); indices.push_back(1); indices.push_back(0);



}