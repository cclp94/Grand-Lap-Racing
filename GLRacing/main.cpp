#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm\ext.hpp>
#include <irrKlang\irrKlang.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>
#include "Models/Model.h"
#include "Models/Plane.h"
#include "Models/kart.h"
#include "Models/Road.h"
#include "Shaders/Shader.h"
#include "Models/Barrier.h"
#include "Light/DirectionalLight.h"
#include "DepthMap/DepthMap.h"
#include "Models/Bridge.h"
#include "Models/StartLine.h"
#include "GameController.h"
#include "Water/Water.h"
#include "Skybox/skybox.h"
#include "Models/Tree.h"
#include "Models\TestQuad.h"
#include "Water\WaterFrameBuffers.h"

using namespace std;

GLuint width = 800, height = 600;

GLFWwindow* window = 0x00;


glm::mat4 view_matrix;
glm::mat4 proj_matrix;


GLfloat point_size = 3.0f;
kart *Kart;
GameController *game;

vector <Model> sceneModels;




bool initialize();
bool cleanUp();
void renderScene(vector<Model*> models);
void cheater();

irrklang::ISoundEngine* engine;
irrklang::ISoundEngine* music;

void keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void windowResizeCallback(GLFWwindow * window, int newWidth, int newHeight);


int main() {
	initialize();
	//Light
	DirectionalLight light(glm::vec3(0.0, 50000000, -50000000), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	//Sound
	engine = irrklang::createIrrKlangDevice();
	music = irrklang::createIrrKlangDevice();

	//Set Shaders
	//Shader *mainShader = new Shader("vertexShader1.vs", "lightFragShader.fs");
	Shader *terrainShader = new Shader("mainVShader.vs", "mainFShader.fs");
	Shader *depthShader = new Shader("depthVShader.vs", "depthFShader.fs");
	Shader *waterShader = new Shader("waterVShader.vs", "waterFShader.fs");
	Shader *skyShader = new Shader("skybox.vs", "skybox.fs");
	Shader *textShader = new Shader("textVShader.vs", "textFShader.fs");

	// Perspective Projection
	proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	terrainShader->use();
	glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
	light.setProperties(terrainShader);

	waterShader->use();
	glUniformMatrix4fv(waterShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
	
	//SkyBox class
	skybox skybox(skyShader);

	// Objects in scene
	Bridge bridge(terrainShader);						
	Plane plane(terrainShader);							
	Road road(terrainShader, &plane);					
	Barrier barrier1(terrainShader, Barrier::OUTTER);	
	Barrier barrier2(terrainShader, Barrier::INNER);	
	Kart = new kart(terrainShader);						
	Tree tree(terrainShader);							
	StartLine start(terrainShader);						
	Water water(waterShader);
	TestQuad quad(terrainShader, -255.0);
	TestQuad quad2(terrainShader, -247.0);
	game = new GameController(textShader, Kart, &start, 3);

	//Shadow Depth Map
	DepthMap depthMap(width, height);
	WaterFrameBuffers waterBuffers(width, height);
	waterBuffers.setClipHeight(water.getHeight());

	music->play2D("Assets/Sounds/GLRacing.wav", true);
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//-------------------------------------------------------------
	float move = 0;
	double currentTime = glfwGetTime();

	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		
		
		// Depth Scene Rendering
		glm::mat4 lightSpaceMatrix = light.getLightSpaceMatrix(width, height);

		// 1. Render depth of scene to texture (from light's perspective)
		// - Get light projection/view matrix.
		// - render scene from light's point of view
		
		depthShader->use();
		glUniformMatrix4fv(depthShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		
		
		depthMap.bind(width, height);
		//--------Render Depth Scene --------------
		plane.depthDraw(depthShader);
		Kart->depthDraw(depthShader);
		road.depthDraw(depthShader);
		barrier1.depthDraw(depthShader);
		barrier2.depthDraw(depthShader);
		bridge.depthDraw(depthShader);
		start.depthDraw(depthShader);
		water.depthDraw(depthShader);
		//------------------------------------------
		depthMap.unbind();


		// wipe the drawing surface clear
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.8f, 0.9f, 0.9f, 1.0f);
		glPointSize(point_size);

		glm::mat4 view = Kart->getCameraSky();

		

		// Projectiion Matrix and light
		terrainShader->use();
		glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		waterShader->use();
		glUniformMatrix4fv(waterShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		
		
		view_matrix = Kart->getCameraView(); // Get Camera View Matrix
		glm::vec3 viewPos = Kart->getCameraPosition();


		//---------------------------------WATER REFRACTION-----------------------------------
		waterBuffers.bindRefractionFBO();

		////Draw background
		//skyShader->use();
		//glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "projection"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		//skybox.draw();


		terrainShader->use();
		proj_matrix = glm::perspective(45.0f,
			(GLfloat)waterBuffers.REFRACTION_WIDTH / (GLfloat)waterBuffers.REFRACTION_HEIHGT,
			0.1f, 1000.0f);

		terrainShader->use();
		glm::vec4 clipPlane = waterBuffers.getRefractionClipPlane();
		glUniform4f(terrainShader->getUniform("plane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
		glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		// Apply view Matrix in secondary shader programs
		glUniform3f(terrainShader->getUniform("viewPos"), viewPos.x, viewPos.y, viewPos.z);
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(terrainShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(terrainShader->getUniform("shadowBias"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap.getId());
		plane.draw();
		barrier1.draw();
		barrier2.draw();
		road.draw();
		Kart->draw();
		bridge.draw();
		start.draw();
		tree.draw();
		quad.setTexture(waterBuffers.getRefractionTexture());
		quad.draw();
		quad2.setTexture(waterBuffers.getReflectionTexture());
		quad2.draw();

		waterBuffers.unBind(width, height);
		
		proj_matrix = glm::perspective(45.0f,
			(GLfloat)waterBuffers.REFLECTION_WIDTH / (GLfloat)waterBuffers.REFLECTION_HEIGHT,
			0.1f, 1000.0f);
		waterBuffers.bindReflectionFBO();

		//Draw background
		skyShader->use();
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "projection"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		skybox.draw();
		
		//-----------------------------------WATER REFLECTION-------------------------
		terrainShader->use();
		//// Apply view Matrix in secondary shader programs
		//Position camera under water
		glm::vec3 underWaterViewPos = viewPos;
		float distance = 2 * (viewPos.y - water.getHeight());
		underWaterViewPos.y = distance;
		glm::mat4 underWaterViewMatrix;// = glm::translate(view_matrix, glm::vec3(0.0, -20, 0.0));
		glm::vec3 pos = Kart->getPosition();
		underWaterViewMatrix = glm::lookAt(underWaterViewPos, glm::vec3(0.0, 0.0,0.0), glm::vec3(0.0, 1.0, 0.0));
		clipPlane = waterBuffers.getReflectionClipPlane();
		glUniform4f(terrainShader->getUniform("plane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
		glUniform3f(terrainShader->getUniform("viewPos"), underWaterViewPos.x, underWaterViewPos.y, underWaterViewPos.z);
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(underWaterViewMatrix));
		glUniformMatrix4fv(terrainShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));		
		
		plane.draw();
		barrier1.draw();
		barrier2.draw();
		road.draw();
		Kart->draw();
		bridge.draw();
		start.draw();
		tree.draw();
		
		waterBuffers.unBind(width, height);
		proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

		//---------------------------------------NORMAL RENDER-----------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.8f, 0.9f, 0.9f, 1.0f);

		//Draw background
		skyShader->use();
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "projection"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		skybox.draw();

		terrainShader->use();
		// Apply view Matrix in secondary shader programs
		glUniform4f(terrainShader->getUniform("plane"), 0.0, 1.0, 0.0, 10000.0);

		glUniform3f(terrainShader->getUniform("viewPos"), viewPos.x, viewPos.y, viewPos.z);
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(terrainShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(terrainShader->getUniform("shadowBias"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap.getId());
		plane.draw();
		barrier1.draw();
		barrier2.draw();
		road.draw();
		Kart->draw();
		bridge.draw();
		start.draw();
		tree.draw();
		quad.setTexture(waterBuffers.getRefractionTexture());
		quad.draw();
		quad2.setTexture(waterBuffers.getReflectionTexture());
		quad2.draw();
		Kart->resetTurnAngle();

		// Main Shader Program
		waterShader->use();
		glUniformMatrix4fv(waterShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterBuffers.getReflectionTexture());
		glUniform1i(waterShader->getUniform("reflectionTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterBuffers.getRefractionTexture());
		glUniform1i(waterShader->getUniform("refractionTexture"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterBuffers.getDuDvMap());
		glUniform1i(waterShader->getUniform("dudvMap"), 2);
		glUniform3f(waterShader->getUniform("cameraPos"), viewPos.x, viewPos.y, viewPos.z);
		
		move += water.getMoveFactor()*(glfwGetTime() - currentTime);
		currentTime = glfwGetTime();
		if (move > 0.5) move = 0.0;
		glUniform1f(waterShader->getUniform("moveFactor"),move);


		water.draw();
		
		game->update(width, height);	
		
	
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
		Kart->move(&plane, &bridge, &road);
		tree.setTurningAngle(Kart->getTurnAngle());
		
	}
	music->drop();
	engine->drop();
	delete textShader, terrainShader, waterShader, Kart, skyShader, game;
	cleanUp();
	return 0;
}

void renderScene(vector<Model*> models) {
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->draw();
	}
}


bool initialize() {
	/// Initialize GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	width = mode->width;
	height = mode->height;
	window = glfwCreateWindow(mode->width, mode->height, "Grand Lap Racing", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	//-------------------------------------------------

	glfwMakeContextCurrent(window);

	//Event Callbacks
	glfwSetKeyCallback(window, keyPressedCallback);

	//Window Resize
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	/// Initialize GLEW extension handler
	glewExperimental = GL_TRUE;	///Needed to get the latest version of OpenGL
	glewInit();

	/// Get the current OpenGL version
	const GLubyte* renderer = glGetString(GL_RENDERER); /// Get renderer string
	const GLubyte* version = glGetString(GL_VERSION); /// Version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/// Enable the depth test i.e. draw a pixel if it's closer to the viewer
	glEnable(GL_DEPTH_TEST); /// Enable depth-testing
	glDepthFunc(GL_LESS);	/// The type of testing i.e. a smaller value as "closer"
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CLIP_DISTANCE0);
	return true;
}

/*
destoys application at closure
*/
bool cleanUp() {
	glDisableVertexAttribArray(0);

	// Close GL context and any other GLFW resources
	glfwTerminate();

	return true;
}

void keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_S) {
		Kart->deaccelerate();
	} else if (key == GLFW_KEY_W) {
		Kart->accelerate();
	}else if (key == GLFW_KEY_A) {
		Kart->turn(0.05);
	}else if(key == GLFW_KEY_D){
		Kart->turn(-0.05) ;
	}else if (key == GLFW_KEY_ESCAPE) {
		glfwWindowShouldClose(window);
		cleanUp();
		exit(1);
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		cheater();
	}

	if ((key == GLFW_KEY_S || key == GLFW_KEY_W) && action == GLFW_RELEASE) {
		Kart->notAccelerating();
	}

	
}

void cheater() {
	string option, mode;
	bool done = false;
	cout << "Input cheat (type help for options): ";
	cin >> option >> mode;
	if (option == "collision") {
		if (mode == "true")
			Kart->setCollision(true);
		else
			Kart->setCollision(false);

	}
	else if (option == "help") {
		cout << "Available options:" << endl;
		cout << "collision [true|false]" << endl;
		cout << "game reset" << endl;
		cout << "laps [number]" << endl;
	}
	else if (option == "game") {
		if (mode == "reset") {
			Kart->resetGame();
			game->reset();
		}
	}
	else if (option == "laps") {
		game->setLaps(atoi(mode.c_str()));
	}

}

void windowResizeCallback(GLFWwindow * window, int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);

	proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}