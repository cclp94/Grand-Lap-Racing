#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm\ext.hpp>

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
#include "Model.h"
#include "Plane.h"
#include "kart.h"
#include "Road.h"
#include "Shader.h"
#include "Barrier.h"
#include "DirectionalLight.h"
#include "DepthMap.h"

using namespace std;

GLuint width = 800, height = 600;

GLFWwindow* window = 0x00;


glm::mat4 view_matrix;
glm::mat4 proj_matrix;


GLfloat point_size = 3.0f;
kart *Kart;

bool initialize();
bool cleanUp();

void keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void windowResizeCallback(GLFWwindow * window, int newWidth, int newHeight);


int main() {
	initialize();
	//Light
	DirectionalLight light(glm::vec3(-1000.0, 1060.0, 0.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

	//Set Shaders
	Shader *mainShader = new Shader("vertexShader1.vs", "lightFragShader.fs");
	Shader *terrainShader = new Shader("terrainVertexShader.vs", "lightFragShader.fs");
	Shader *depthShader = new Shader("depthVShader.vs", "depthFShader.fs");

	// Perspective Projection
	proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);


	// Projectiion Matrix and light
	mainShader->use();
	glUniformMatrix4fv(mainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
	light.setProperties(mainShader);

	terrainShader->use();
	glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
	light.setProperties(terrainShader);
	

	// Objects in scene
	Plane plane(terrainShader);
	Road road(terrainShader);
	Barrier barrier1(terrainShader, Barrier::OUTTER);
	Barrier barrier2(terrainShader, Barrier::INNER);
	Kart = new kart(terrainShader);

	//Shadow Depth Map
	DepthMap depthMap;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//-------------------------------------------------------------


	// Game Loop
	while (!glfwWindowShouldClose(window)) {

		// Depth Scene Rendering
		glm::mat4 lightSpaceMatrix = light.getLightSpaceMatrix(width, height);

		// 1. Render depth of scene to texture (from light's perspective)
		// - Get light projection/view matrix.
		// - render scene from light's point of view
		
		depthShader->use();
		glUniformMatrix4fv(depthShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		
		
		depthMap.bind();
		//--------Render Depth Scene --------------
		plane.depthDraw(depthShader);
		Kart->depthDraw(depthShader);
		road.depthDraw(depthShader);
		barrier1.depthDraw(depthShader);
		barrier2.depthDraw(depthShader);
		//------------------------------------------
		depthMap.unbind();


		// wipe the drawing surface clear
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.8f, 0.9f, 0.9f, 1.0f);
		glPointSize(point_size);

		// Projectiion Matrix and light
		mainShader->use();
		glUniformMatrix4fv(mainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
		terrainShader->use();
		glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));

		view_matrix = Kart->getCameraView(); // Get Camera View Matrix
		glm::vec3 viewPos = Kart->getCameraPosition();

		terrainShader->use();
		// Apply view Matrix in secondary shader programs
		glUniform3f(terrainShader->getUniform("viewPos"), viewPos.x, viewPos.y, viewPos.z);
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(terrainShader->getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap.getId());
		plane.draw();
		barrier1.draw();
		barrier2.draw();
		road.draw();
		Kart->draw();

		// Main Shader Program
		mainShader->use();
		glUniformMatrix4fv(mainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		
		
		
		
	
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}
	delete mainShader, terrainShader;
	cleanUp();
	return 0;
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

	if ((key == GLFW_KEY_S || key == GLFW_KEY_W) && action == GLFW_RELEASE) {
		Kart->notAccelerating();
	}

	
}

void windowResizeCallback(GLFWwindow * window, int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);

	proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}