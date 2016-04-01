#include "skybox.h"


skybox::skybox(Shader *s):Model(s)
{
	color = glm::vec3(0.2, 0.5, 0.2);
	getModel();
	setupMesh();
	model_matrix = glm::scale(model_matrix, glm::vec3(10));

}

skybox::~skybox()
{
}

void skybox::getModel() 
{
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);

	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);

	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);

	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);
	
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(1.0f);
	vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(-1.0f);
	
	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(-1.0f);
	vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);
	vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(1.0f);

	vector<const GLchar*> faces;
	faces.push_back("Assets/skybox/right.jpg");
	faces.push_back("Assets/skybox/left.jpg");
	faces.push_back("Assets/skybox/top.jpg");
	faces.push_back("Assets/skybox/bottom.jpg");
	faces.push_back("Assets/skybox/back.jpg");
	faces.push_back("Assets/skybox/front.jpg");
	texture = loadCubemap(faces);
}

GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void skybox::draw() 
{
	glDepthMask(GL_FALSE);// Remember to turn depth writing off
	
	glUseProgram(shaderProgram->programID);
	//this->shaderProgram->use();

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram->programID, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);

	
}

void skybox::depthDraw(Shader *s) {
	glBindVertexArray(VAO);
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


GLuint skybox::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

