#include "kart.h"
#include "Camera.h"


kart::~kart() {
	delete camera;
}



kart::kart(Shader *s) : Model(s){
	color = glm::vec3(0.3, 0.1, 0.2);
	position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	maxSpeed = 1.0f;
	acceleration = 0.05f;
	speed = 0.0f;
	isAccelarating = false;
	getModel();
	setupMesh();
	camera = new Camera();
	camera->cameraPos = glm::vec3(position.x, position.y + 1, position.z + 3.0);

	model_matrix = glm::translate(model_matrix, glm::vec3(-250.0, 0.0, 0.0));
}

void kart::move() {
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, -getSpeed()));
	glm::vec4 previousPos = position;
	position = model_matrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	camera->cameraPos = glm::vec3(model_matrix * glm::vec4(0.0f, 1.0f, 3.0f, 1.0f));

	update();
}

void kart::turn(float angle) {
	if (speed > 0) {
		model_matrix = glm::rotate(model_matrix, angle, glm::vec3(0.0, 1.0, 0.0));
	}
}

glm::mat4 kart::getCameraView() {
	view_matrix = glm::lookAt(camera->cameraPos,glm::vec3(position),camera->cameraUp);
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

void kart::draw() {
	move();

	glBindVertexArray(VAO);
	glUniformMatrix4fv(shaderProgram->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));

	glUniform3f(shaderProgram->getUniform("vertex_color"), color.x, color.y, color.z);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(shaderProgram->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
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
		indices.push_back(7); indices.push_back(4); indices.push_back(2);
		indices.push_back(0); indices.push_back(3); indices.push_back(1);
		indices.push_back(6); indices.push_back(5); indices.push_back(4);
		indices.push_back(1); indices.push_back(0);



}