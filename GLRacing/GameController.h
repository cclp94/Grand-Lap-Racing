#pragma once
#include "Model.h"

class GameController
{
public:
	GameController(Model *character, Model *start, int laps);
	~GameController();
	void update();

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
};

