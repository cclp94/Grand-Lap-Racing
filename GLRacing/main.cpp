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

#include "kart.h"
#include "Shader.h"

using namespace std;

GLFWwindow* window = 0x00;

GLuint shader_program = 0;

GLuint view_matrix_id = 0;
GLuint model_matrix_id = 0;
GLuint proj_matrix_id = 0;

glm::mat4 proj_matrix;
glm::mat4 view_matrix;
glm::mat4 model_matrix;


//Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLuint VBO, VAO, EBO;

GLfloat point_size = 3.0f;
kart Kart;

bool initialize();
bool cleanUp();
GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path);

void keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main() {
	initialize();

	///Load the shaders
	shader_program = loadShaders("vertexShader1.vs", "fragmentShader1.fs");

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, Kart.getModelsize(), Kart.getModelVertices(), GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_FLOAT)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);



	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glPointSize(point_size);

		glUseProgram(shader_program);


		view_matrix = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), cameraUp);


		proj_matrix = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);


		//Pass the values of the three matrices to the shaders
		glUniformMatrix4fv(proj_matrix_id, 1, GL_FALSE, glm::value_ptr(proj_matrix));
		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, glm::value_ptr(model_matrix));

		glBindVertexArray(VAO);

		Kart.update();

		model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, -Kart.getSpeed()));
		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

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
	window = glfwCreateWindow(800, 600, "Grand Lap Racing", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	//-------------------------------------------------

	glfwMakeContextCurrent(window);

	//Event Callbacks
	glfwSetKeyCallback(window, keyPressedCallback);

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
	//Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Close GL context and any other GLFW resources
	glfwTerminate();

	return true;
}

/*
Load shaders from given files and creates them
*/
GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path) {
	Shader *shader = new Shader(vertex_shader_path, 0);
	// Create the shaders
	GLuint VertexShaderID = shader->getShaderId();
	delete shader;
	shader = new Shader(fragment_shader_path, 1);
	GLuint FragmentShaderID = shader->getShaderId();

	
	GLuint ProgramID = glCreateProgram();
	ProgramID = shader->makeProgram(VertexShaderID, FragmentShaderID);

	delete shader;
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	//The three variables below hold the id of each of the variables in the shader
	//If you read the vertex shader file you'll see that the same variable names are used.
	view_matrix_id = glGetUniformLocation(ProgramID, "view_matrix");
	model_matrix_id = glGetUniformLocation(ProgramID, "model_matrix");
	proj_matrix_id = glGetUniformLocation(ProgramID, "proj_matrix");

	return ProgramID;
}

void keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_S) {
		Kart.deaccelerate();
	}
	if (key == GLFW_KEY_W) {
		Kart.accelerate();
	}
	if (key == GLFW_KEY_A) {
		model_matrix = glm::rotate(model_matrix, 0.05f, glm::vec3(0.0, 1.0, 0.0));
	}
	if(key == GLFW_KEY_D){
		model_matrix = glm::rotate(model_matrix, -0.05f, glm::vec3(0.0, 1.0, 0.0));
	}

	if ((key == GLFW_KEY_S || key == GLFW_KEY_W) && action == GLFW_RELEASE) {
		Kart.notAccelerating();
	}
}