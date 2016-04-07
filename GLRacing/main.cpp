#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm\ext.hpp>
#include <irrKlang\irrKlang.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>

//Shader
#include "Shaders/Shader.h"

//Models
#include "Models/Model.h"
#include "Models/Plane.h"
#include "Models/kart.h"
#include "Models/Road.h"
#include "Models/Barrier.h"
#include "Models/Bridge.h"
#include "Models/BillboardModel.h"
#include "Models/Windmill.h"
#include "Models/Fence.h"

//SkyBox
#include "Skybox/skybox.h"

// Water
#include "Water/WaterFrameBuffers.h"
#include "Water/Water.h"

//Game Controller
#include "GameController.h"
// Light
#include "Light/DirectionalLight.h"

//Depth Map
#include "DepthMap/DepthMap.h"
#include "TextureRenderer.h"

using namespace std;

GLuint width = 800, height = 600;

GLFWwindow* window = 0x00;


glm::mat4 view_matrix;
glm::mat4 proj_matrix;


GLfloat point_size = 3.0f;
kart *Kart;
GameController *game;

vector <Model*> sceneModels;




bool initialize();
bool cleanUp();
void renderScene();
void cheater();
void renderForest(BillboardModel *tree, glm::vec3 initial, float scale, float offset, int width, int height);
void renderBleachers(ImportedModel *bl, glm::vec3 pos1, glm::vec3 pos2, float rot);
void renderWindmills(ImportedModel *wind, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, float scale, float rot);
GLuint renderToTexture(Shader *s, Model *model);
void renderDuckFamily(vector<BillboardModel*> &ducks);
void renderFarm(vector<BillboardModel*> &farmAnimals);
void renderRandomGrid(vector<BillboardModel*> &models, glm::vec3 initial, float scale, float offset, int width, int height, int random);

irrklang::ISoundEngine* engine;
irrklang::ISoundEngine* music;

void keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void windowResizeCallback(GLFWwindow * window, int newWidth, int newHeight);


int main() {
	initialize();
	
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

	//Light
	DirectionalLight light(glm::vec3(0.0, 50000000, -50000000), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	light.setProperties(terrainShader);

	// Perspective Projection Matrix
	proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	
	//SkyBox class
	skybox skybox(skyShader);

	// Objects in scene and Push   them into render vector
	Kart = new kart(terrainShader);		sceneModels.push_back(Kart);

	Bridge bridge(terrainShader);		sceneModels.push_back(&bridge);
	Plane plane(terrainShader);			sceneModels.push_back(&plane);
	Road road(terrainShader, &plane);	sceneModels.push_back(&road);
	Barrier barrier1(terrainShader, Barrier::OUTTER);	sceneModels.push_back(&barrier1);
	Barrier barrier2(terrainShader, Barrier::INNER);	sceneModels.push_back(&barrier2);	
	ImportedModel start(terrainShader, "Assets/Startline/violet_car.obj", glm::vec3(-250.0, 0.0, 0.0), glm::vec3(2), 0);		sceneModels.push_back(&start);
	Water water(waterShader);
	ImportedModel bleachers(terrainShader, "Assets/Bleachers/generic\ medium.obj", glm::vec3(-215.0, 0.0, -50.0), glm::vec3(1), -90);
	ImportedModel building(terrainShader, "Assets/Apartment/simple\ building.obj", glm::vec3(0.0), glm::vec3(20), 90);
	sceneModels.push_back(&building);

	BillboardModel tree(terrainShader, glm::vec3(-220.0, -1, 0.0), glm::vec3(20), "tree.png", Kart->getTurnAngle());

	//Farm

	//Render Corn and generate Texture of it
	ImportedModel corn(terrainShader, "Assets/corn/corn.obj", glm::vec3(0.0, -1.5, -2), glm::vec3(1), 0);
	GLuint cornTexture = renderToTexture(terrainShader, &corn);
	BillboardModel cornBill(terrainShader, glm::vec3(-220.0, 0, 40.0), glm::vec3(3), cornTexture, Kart->getTurnAngle());

	BillboardModel cow(terrainShader, glm::vec3(270.0, 0, -200.0), glm::vec3(2), "animals/Cow.png", Kart->getTurnAngle());
	BillboardModel cow2(terrainShader, glm::vec3(270.0, 0, -200.0), glm::vec3(2), "animals/Cow2.png", Kart->getTurnAngle());
	sceneModels.push_back(&cow);
	sceneModels.push_back(&cow2);
	vector<BillboardModel*> farmAnimals;
	farmAnimals.push_back(&cow);
	farmAnimals.push_back(&cow2);
	Windmill windmill(terrainShader, "Assets/windmill/windmil.obj", glm::vec3(200, 0.0, 0.0), glm::vec3(5), -90.0);

	// Lake
	ImportedModel bench(terrainShader, "Assets/Bench/bench.obj", glm::vec3(10.0, 0.0, -200), glm::vec3(1.2), 90);
	sceneModels.push_back(&bench);
	ImportedModel lakeHouse(terrainShader, "Assets/lakeHouse/Farmhouse\ OBJ.obj", glm::vec3(50.0, 0.0, -400), glm::vec3(0.7), 180);
	sceneModels.push_back(&lakeHouse);
	BillboardModel duck(terrainShader, glm::vec3(270.0, 0, -200.0), glm::vec3(2), "animals/Duck.png", Kart->getTurnAngle());
	BillboardModel duck2(terrainShader, glm::vec3(270.0, 0, -200.0), glm::vec3(2), "animals/Duck2.png", Kart->getTurnAngle());
	BillboardModel duck3(terrainShader, glm::vec3(270.0, 0, -200.0), glm::vec3(2), "animals/babyduck.png", Kart->getTurnAngle());
	
	vector<BillboardModel*> ducks;
	ducks.push_back(&duck);
	ducks.push_back(&duck2);
	ducks.push_back(&duck3);
	


	Fence fence(terrainShader, glm::vec3(300.0, 0.1, -100.0), 20);
	sceneModels.push_back(&fence);

	game = new GameController(textShader, Kart, &start, 3);

	//Shadow Depth Map
	DepthMap depthMap(width, height);
	WaterFrameBuffers waterBuffers(width, height);
	waterBuffers.setClipHeight(water.getHeight());

	music->play2D("Assets/Sounds/GLRacing.wav", true);
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	// Water movement Variables-------
	float move = 0;
	double currentTime = glfwGetTime();
	//---------------------------------
	srand(time(NULL));
	int random = rand();

	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		
		
		// View- Projection Matrix from light perspective
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
		windmill.depthDraw(depthShader);
		//------------------------------------------
		depthMap.unbind();


		// wipe the drawing surface clear
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.8f, 0.9f, 0.9f, 1.0f);
		glPointSize(point_size);


		// View Matrix for Skybox
		glm::mat4 view = Kart->getCameraSky();

		// Projectiion Matrix and light
		terrainShader->use();
		glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		waterShader->use();
		glUniformMatrix4fv(waterShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		
		
		view_matrix = Kart->getCameraView();				// Get Camera View Matrix
		glm::vec3 viewPos = Kart->getCameraPosition();		// Camera Position


		//---------------------------------WATER REFRACTION-----------------------------------
		waterBuffers.bindRefractionFBO();		// Bind Water Refraction Framebuffer Object


		// Adjust projection
		proj_matrix = glm::perspective(45.0f,
			(GLfloat)waterBuffers.REFRACTION_WIDTH / (GLfloat)waterBuffers.REFRACTION_HEIHGT,
			0.1f, 1000.0f);

		terrainShader->use();
		glm::vec4 clipPlane = waterBuffers.getRefractionClipPlane();		// Set Clip Plane
		glUniform4f(terrainShader->getUniform("plane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
		glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		glUniform3f(terrainShader->getUniform("viewPos"), viewPos.x, viewPos.y, viewPos.z);
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(terrainShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glActiveTexture(GL_TEXTURE1);					// DepthMap Texture
		glBindTexture(GL_TEXTURE_2D, depthMap.getId());
		glUniform1i(terrainShader->getUniform("shadowMap"), 1);
		//Render Relevant scene
		renderScene();
		renderDuckFamily(ducks);

		waterBuffers.unBind(width, height);		// Unbind FBO

		//-----------------------------------WATER REFLECTION-------------------------
		
		// Readjust Projection matrix for reflection
		proj_matrix = glm::perspective(45.0f,
			(GLfloat)waterBuffers.REFLECTION_WIDTH / (GLfloat)waterBuffers.REFLECTION_HEIGHT,
			0.1f, 1000.0f);
		//Bind FBO
		waterBuffers.bindReflectionFBO();

		//Draw Skybox
		skyShader->use();
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "projection"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		skybox.draw();
		
		
		terrainShader->use();
		//Position camera under water
		glm::vec3 underWaterViewPos = viewPos;
		float distance = 2 * (viewPos.y - water.getHeight());
		underWaterViewPos.y = distance;
		glm::mat4 underWaterViewMatrix;
		glm::vec3 pos = Kart->getPosition();
		underWaterViewMatrix = glm::lookAt(underWaterViewPos, glm::vec3(0.0, 0.0,0.0), glm::vec3(0.0, 1.0, 0.0));

		clipPlane = waterBuffers.getReflectionClipPlane();	// Set clip plane to clip bellow water
		glUniform4f(terrainShader->getUniform("plane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
		glUniform3f(terrainShader->getUniform("viewPos"), underWaterViewPos.x, underWaterViewPos.y, underWaterViewPos.z);
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(underWaterViewMatrix));
		glUniformMatrix4fv(terrainShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));		
		
		// Render
		renderScene();
		renderForest(&tree, glm::vec3(-470, -1, 440.0), 20, 20, 7, 26);
		renderForest(&cornBill, glm::vec3(300.0, 0, 50.0), 7, 5, 5, 15);
		renderDuckFamily(ducks);
		
		waterBuffers.unBind(width, height);

		//---------------------------------------NORMAL RENDER-----------------------------------

		// Readjust Projection Matrix
		proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

		//Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.8f, 0.9f, 0.9f, 1.0f);

		//Draw Skybox
		skyShader->use();
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyShader->programID, "projection"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		skybox.draw();

		terrainShader->use();

		glUniform4f(terrainShader->getUniform("plane"), 0.0, 1.0, 0.0, 10000.0);

		glUniform3f(terrainShader->getUniform("viewPos"), viewPos.x, viewPos.y, viewPos.z);
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(terrainShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(terrainShader->getUniform("shadowBias"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		// DepthMap
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap.getId());

		// Render Scene
		renderScene();
		renderForest(&tree, glm::vec3(-470, -1, 440.0), 20, 20, 7, 26);
		renderForest(&cornBill, glm::vec3(300.0, 0, 50.0), 7, 5, 5, 15);
		renderDuckFamily(ducks);
		renderFarm(farmAnimals);
		renderBleachers(&bleachers, glm::vec3(-215.0, 0.0, -50.0), glm::vec3(-275.0, 0.0, -50.0), 90);
		renderWindmills(&windmill, glm::vec3(200, 0.0, 0.0), glm::vec3(220, 0.0, 30.0), glm::vec3(220, 0.0, -30.0), 5, -90);
		// ------------------------------------Water---------------------------------

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
			// Water Moviment
			move += water.getMoveFactor()*(glfwGetTime() - currentTime);
			currentTime = glfwGetTime();
			if (move > 0.5) move = 0.0;
			glUniform1f(waterShader->getUniform("moveFactor"),move);

			// Render
			water.draw();

		//--------------------------------------------------------------------------------


		// Update Game Controller
		game->update(width, height);	
		
	
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
		Kart->move(&plane, &bridge, &road);		// Update Kart moviment
		
	}

	// Clear alocated Memory
	music->drop();
	engine->drop();
	delete textShader, terrainShader, waterShader, Kart, skyShader, game;
	cleanUp();
	return 0;
}


/*
	Renders objects inside the sceneModels vector
*/
void renderScene() {
	for (int i = 0; i < sceneModels.size(); i++)
	{
		sceneModels[i]->draw();
	}
}


/*
	Render Generic Billboards in a grid formation
	@param tree pointer to Billboard model
	@param initial Initial bottom left position for grid
	@param scale scale factor for model transformation
	@param offset How far model are from each other
	@param width number of objects in the X direction
	@param height number of object in the z direction
*/
void renderForest(BillboardModel *tree, glm::vec3 initial, float scale, float offset, int width, int height) {
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			glm::vec3 pos = glm::vec3(initial.x + (j * offset), initial.y, initial.z - i*(offset));
			tree->setPosition(pos, glm::vec3(scale));	// Move object
			tree->draw();								// Render Instance
		}
	}
}


/*
	Render Generic multiple billboards in a ramdom grid formation
	@param tree pointer to Billboard model
	@param initial Initial bottom left position for grid
	@param scale scale factor for model transformation
	@param offset How far model are from each other
	@param width number of objects in the X direction
	@param height number of object in the z direction
*/
void renderRandomGrid(vector<BillboardModel*> &models, 
	glm::vec3 initial, float scale, float offset, int width, int height, int random) {
	int numberOfEntities = models.size();
	int place = random % numberOfEntities;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			
			glm::vec3 pos = glm::vec3(initial.x + (j * offset), initial.y, initial.z + (offset / 2 * (j % 2)) - i*(offset));
			models[(place + (j * i) + random) % numberOfEntities]->setPosition(pos, glm::vec3(scale));	// Move object
			models[(place + (j * i) + random) % numberOfEntities]->draw();								// Render Instance
		}
	}
	
}


/*
	Renders 2 instances of Bleacher facing each other 
*/
void renderBleachers(ImportedModel *bl, glm::vec3 pos1, glm::vec3 pos2, float rot) {
	bl->setPosition(pos1, glm::vec3(1), -rot);
	bl->draw();
	bl->setPosition(pos2, glm::vec3(1), rot);
	bl->draw();
}


/*
	Renders to Framebuffer and return a texture of the model
	@param s Shader program used by the model
	@param model model to be rendered
*/
GLuint renderToTexture(Shader *s, Model *model) {
	TextureRenderer t(width, height);
	// Set simple View Matrix
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 3.0, 3), glm::vec3(0.0, 1.5, 0.0), glm::vec3(0.0, 0.0, 1.0));
	t.bind();	// Bind FBO
	s->use();	// Use Shader
	glUniformMatrix4fv(s->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(s->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
	model->draw();		// Render
	t.unbind(width, height);	// Unbind FBO
	s->unuse();			// Unbind shader

	return t.getTexture();		// Return Texture ID
}
/*
	Render animal in the farm
	@param farmAnimals Vector of animals to be rendered
*/
void renderFarm(vector<BillboardModel*> &farmAnimals) {
	farmAnimals[0]->setPosition(glm::vec3(290.0, 0.0, -90.0), glm::vec3(2));
	farmAnimals[0]->draw();
	farmAnimals[0]->setPosition(glm::vec3(300.0, 0, -100), glm::vec3(2));
	farmAnimals[0]->draw();
	farmAnimals[0]->setPosition(glm::vec3(310.0, 0, -100), glm::vec3(2));
	farmAnimals[0]->draw();
	farmAnimals[0]->setPosition(glm::vec3(300.0, 0, -110), glm::vec3(2));
	farmAnimals[0]->draw();


	farmAnimals[1]->setPosition(glm::vec3(290, 0, -100.0), glm::vec3(2));
	farmAnimals[1]->draw();
	farmAnimals[1]->setPosition(glm::vec3(310, 0, -90.0), glm::vec3(2));
	farmAnimals[1]->draw();
	farmAnimals[1]->setPosition(glm::vec3(300, 0, -90.0), glm::vec3(2));
	farmAnimals[1]->draw();
}


/*
	Render family of ducks in the lake
*/
void renderDuckFamily(vector<BillboardModel*> &ducks) {

	ducks[0]->setPosition(glm::vec3(90.0, -5.5, -306.0), glm::vec3(4));
	ducks[0]->draw();
	ducks[0]->setPosition(glm::vec3(88.0, -5.5, -303.0), glm::vec3(2));
	ducks[0]->draw();
	ducks[0]->setPosition(glm::vec3(86.0, -5.5, -302.0), glm::vec3(2));
	ducks[0]->draw();
	ducks[0]->setPosition(glm::vec3(86.0, -5.5, -301.0), glm::vec3(2));
	ducks[0]->draw();
	ducks[0]->setPosition(glm::vec3(86.0, -5.5, -300.0), glm::vec3(2));
	ducks[0]->draw();


	ducks[1]->setPosition(glm::vec3(50.0, -5.9, -220.0), glm::vec3(4));
	ducks[1]->draw();

	ducks[2]->setPosition(glm::vec3(50.0, -5.5, -217.0), glm::vec3(2));
	ducks[2]->draw();
	ducks[2]->setPosition(glm::vec3(50.0, -5.5, -215.0), glm::vec3(2));
	ducks[2]->draw();
	ducks[2]->setPosition(glm::vec3(50.0, -5.5, -212.0), glm::vec3(2));
}


/*
	Render 3 instances of Windmills in the scene
	@param wind Windmill object to render
	@param pos1, pos2, pos3 position of windMills
	@param scale scale value for windmill
	@rot rotation angle for transformation
*/
void renderWindmills(ImportedModel *wind, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, float scale, float rot) {
	wind->setPosition(pos1, glm::vec3(scale), rot);
	wind->draw();
	wind->setPosition(pos2, glm::vec3(scale), rot);
	wind->draw();
	wind->setPosition(pos3, glm::vec3(scale), rot);
	wind->draw();
}


/*
	Initializes GLFW and GLEW , creates full screen window, 
	set event callbacks, and enables features used by the
	program such as Depth testing, Multisapling antialiasing, etc;
*/
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
/*
	Keyboard callback
*/
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
/*
	Cheater Feature changes variables in the program
*/
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


/*
	Window resize callback
*/
void windowResizeCallback(GLFWwindow * window, int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);

	proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}