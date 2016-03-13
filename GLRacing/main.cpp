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
	Shader *mainShader = new Shader("vertexShader1.vs", "fragmentShader1.fs");
	Shader *terrainShader = new Shader("terrainVertexShader.vs", "fragmentShader1.fs");


	//The three variables below hold the id of each of the variables in the shader
	//If you read the vertex shader file you'll see that the same variable names are used.

	proj_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	

	mainShader->use();
	glUniformMatrix4fv(mainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
	terrainShader->use();
	glUniformMatrix4fv(terrainShader->getUniform("proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));

	Plane plane(terrainShader);
	Road road(mainShader);
	Kart = new kart(mainShader);

	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.8f, 0.9f, 0.9f, 1.0f);
		glPointSize(point_size);

		view_matrix = Kart->getCameraView();

		terrainShader->use();
		glUniformMatrix4fv(terrainShader->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));

		plane.draw();

		mainShader->use();
		road.draw();
		Kart->draw();
	
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

	/// Create a window of size 640x480 and with title "Lecture 2: First Triangle"
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

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