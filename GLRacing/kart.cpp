#include "kart.h"
#include "Camera.h"

kart::~kart() {
	delete camera;
}

glm::vec3 kart::getCameraPosition() {
	return camera->cameraPos;
}



kart::kart(Shader *s) : ImportedModel(s){
	color = glm::vec3(0.3, 0.1, 0.2);
	material.ambient = glm::vec4(1, 1, 1, 1.0);
	material.diffuse = glm::vec4(1, 1, 1,1.0);
	material.specular = glm::vec4(1, 1, 1, 1.0);
	material.shininess = 76.8;
	position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	maxSpeed = 0.5f;
	acceleration = 0.05f;
	speed = 0.0f;
	isAccelarating = false;
	getModel("Assets/enzo\'s Car/enzo_car.obj");
	//setupMesh();
	camera = new Camera();
	camera->cameraPos = glm::vec3(position.x, position.y + 2, position.z + 5.0);

	model_matrix = glm::translate(model_matrix, glm::vec3(-250.0, 0.0, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(0.7));
	model_matrix = glm::rotate(model_matrix,(float) glm::radians(180.0), glm::vec3(0.0, 1.0, 0.0));
}

void kart::move() {
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, getSpeed()));
	glm::vec4 previousPos = position;
	position = model_matrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	camera->cameraPos = glm::vec3(model_matrix * glm::vec4(0.0f, 5.0f, -15.0f, 1.0f));

	update();
}

void kart::turn(float angle) {
	if (speed != 0) {
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
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform3f(shaderProgram->getUniform("viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	//this->setMaterialUniform();
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}

void kart::depthDraw(Shader *s) {
	move();
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}

