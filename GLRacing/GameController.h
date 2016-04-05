#pragma once
#include "Models/Model.h"
#include <iostream>
#include <map>
#include <irrKlang\irrKlang.h>
class Character;

class GameController
{
public:
	GameController(Shader *s, Model *character, Model *start, int laps);
	~GameController();
	void update(int width, int height);
	void reset();
	void setLaps(int n);

private:
	Model *character;
	glm::vec3 lapPosition;
	const float threshold =10.0f;
	int laps;
	bool gameStarted;
	double seconds;
	int minutes;
	int currentLap;
	pair<int,double> lapTime;
	pair<int,double> startTime;
	pair<int, double> finalTime;
	pair<int, double> recordTime;
	std::map<GLchar, Character> Characters;
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale,glm::vec3 color);
	void setupRender();
	GLuint VAO, VBO;
	Shader *shader;
	irrklang::ISoundEngine* gameSounds;
	irrklang::ISound* complete;
	bool beatRecord;
	void writeRecord();
};

