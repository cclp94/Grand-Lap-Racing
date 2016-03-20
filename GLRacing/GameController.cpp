#include "GameController.h"



GameController::GameController(Model *character, Model *start, int laps)
{
	this->character = character;
	this->lapPosition = start->getPosition();
	this->laps = laps;
	gameStarted = false;
	seconds = 0;
	minutes = 0;
	currentLap = 0;
	finaltime = 0.0;
}


GameController::~GameController()
{
}

void GameController::update() {
	glm::vec3 currentPos = character->getPosition();
	float zthresholdUp = lapPosition.z + threshold;
	float zthresholdDown = lapPosition.z - threshold;
	float xthresholdUp = lapPosition.x + threshold;
	float xthresholdDown = lapPosition.x - threshold;
	if (currentPos.z < zthresholdUp && currentPos.z > zthresholdDown) {
		if (currentPos.x < xthresholdUp && currentPos.x > xthresholdDown) {
			if (finaltime == 0.0) {
				if (!gameStarted) {

					startTime = glfwGetTime();
					currentLap = 0;

					gameStarted = true;
					cout << "Game Started!!" << endl;
				}
				else {

					double currentTime = glfwGetTime();
					if ((currentLap == 0 && currentTime - startTime > 10) ||
						currentLap > 0 && currentTime - lapTime > 10) {
						lapTime = currentTime;
						currentLap++;
						cout << "Lap " << currentLap - 1 << ": " << lapTime - startTime << endl;
						if (currentLap == laps) {
							gameStarted = false;
							finaltime = currentTime - startTime;
							cout << "Finished!! -- " << finaltime << endl;
							//Save result in file
						}
					}

				}
			}
		}
	}
}