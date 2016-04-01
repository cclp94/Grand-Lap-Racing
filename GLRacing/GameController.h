#pragma once
#include "Models/Model.h"
#include <iostream>
#include <map>
class Character;

class GameController
{
public:
	GameController(Shader *s, Model *character, Model *start, int laps);
	~GameController();
	void update(int width, int height);

private:
	Model *character;
	glm::vec3 lapPosition;
	const float threshold =10.0f;
	int laps;
	bool gameStarted;
	int seconds, minutes;
	int currentLap;
	double lapTime;
	double startTime;
	double finaltime;
	std::map<GLchar, Character> Characters;
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale,glm::vec3 color);
	void setupRender();
	GLuint VAO, VBO;
	Shader *shader;

};

