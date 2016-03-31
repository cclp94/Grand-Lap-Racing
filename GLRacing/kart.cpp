#include "kart.h"
#include "Camera.h"


kart::~kart() {
	delete camera;
}

glm::vec3 kart::getCameraPosition() {
	return camera->cameraPos;
}

float kart::getTurnAngle() {
	return turnAngle;
}

void kart::resetTurnAngle() {
	turnAngle = 0.0;
}


kart::kart(Shader *s) : ImportedModel(s){
	color = glm::vec3(0.3, 0.1, 0.2);
	material.ambient = glm::vec4(1, 1, 1, 1.0);
	material.diffuse = glm::vec4(1, 1, 1,1.0);
	material.specular = glm::vec4(1, 1, 1, 1.0);
	material.shininess = 76.8;
	position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	maxSpeed = 10.0f;
	acceleration = 3.0f;
	speed = 0.0f;
	isAccelarating = false;
	getModel("Assets/Enzo\'s Car/enzo_car.obj");
	//setupMesh();
	camera = new Camera();
	camera->cameraPos = glm::vec3(position.x, position.y + 2, position.z + 5.0);

	model_matrix = glm::translate(model_matrix, glm::vec3(-250.0, 5.0, 50.0));
	model_matrix = glm::rotate(model_matrix,(float) glm::radians(180.0), glm::vec3(0.0, 1.0, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(0.4));

	position = model_matrix * position;

	collision = false;
}

void kart::setCollision(bool col) {
	collision = col;
}

void kart::move(Plane *terrain, Bridge *b, Road *r) {
	pair<float, glm::vec3> heightAndNormal;
	float height = terrain->getHeight(position.x, position.z);
	float height2 = b->getHeight(position.x, position.z);
	
	//cout << height2 << endl;
	float yMove;
	if (height < 0 && height2 < 0) {
		yMove = 0;
	}
	else {
		if (height2 > height) {
			if (height2 > 0) {
				yMove = height2 - position.y;
			}
		}
		else {
			if (position.y <= height) {
				isInAir = false;
				yMove = height - position.y;
			}
			else {
				yMove = height - position.y;
			}
		}
	}
	//cout << inBound << endl;
	float zMove;
	if (collision) {
		bool inBound = r->checkBound(position.x, position.z);
		if (!inBound) {
			//model_matrix = glm::rotate(model_matrix, glm::radians(10.0f * inBound.second), glm::vec3(0.0, 1.0, 0.0));
			if (speed < 0)
				model_matrix = glm::translate(model_matrix, glm::vec3(0.0, yMove, 5.0));
			else
				model_matrix = glm::translate(model_matrix, glm::vec3(0.0, yMove, -3.0));

			speed = 0;
		}
	}
	zMove = getSpeed();
	
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0, yMove, zMove));

	previousPos = position;
	position = model_matrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	currentHeight = previousPos.y;
	camera->cameraPos = glm::vec3(model_matrix * glm::vec4(0.0f, 5.0f, -15.0f, 1.0f));


	update();
}

void kart::turn(float angle) {
	if (speed != 0) {
		turnAngle = angle;
		model_matrix = glm::rotate(model_matrix, angle, glm::vec3(0.0, 1.0, 0.0));
	}
}

glm::mat4 kart::getCameraView() {
	view_matrix = glm::lookAt(camera->cameraPos,glm::vec3(position) ,camera->cameraUp);
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
	return speed / 1000;
}

void kart::update() {
	if (!isAccelarating) {
		if (speed < 0) {
			speed += 1;
		}
		else if (speed > 0) {
			speed -= 1;
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
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform3f(shaderProgram->getUniform("viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	//this->setMaterialUniform();
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}

void kart::depthDraw(Shader *s) {
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}

glm::mat4 kart::getCameraSky()
{
	GLfloat posz = camera->cameraPos.z / 1000.0f;
	if (camera->cameraPos.z == 0)
		posz = 0.1f;
	GLfloat posx = camera->cameraPos.x / 1000.0f;
	GLfloat posy = camera->cameraPos.y / 1000.0f;

	GLfloat posx1 = position.x / 1000.0f;
	GLfloat posy1 = position.y / 1000.0f;
	GLfloat posz1 = position.z / 1000.0f;
	return glm::lookAt(glm::vec3(posx, posy, posz), glm::vec3(posx1, posy1, posz1), glm::vec3(0.0f, 1.0f, 0.0f));
}

