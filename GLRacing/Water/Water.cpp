#include "Water.h"

GLuint Water::getRefractionTexture() {
	return refractionTexture;
}

GLuint Water::getReflectionTexture() {
	return reflectionTexture;
}

Water::Water(Shader *s, int displayWidth, int displayHeihgt) : Model(s)
{
	this->displayHeihgt = displayHeihgt;
	this->displayWidth = displayWidth;
	color = glm::vec3(0.0, 1.0, 1.0);
	material.ambient = glm::vec4(color, 0.5);
	material.diffuse = glm::vec4(color, 0.5);
	material.specular = glm::vec4(color, 0.5);
	material.shininess = 1;
	getModel();
	initializeRefractBuffer();
	initializeReflectBuffer();
	setupMesh();
	model_matrix = glm::rotate(model_matrix, glm::radians(-10.0f), glm::vec3(0.0, 1.0, 0.0));
	model_matrix = glm::translate(model_matrix, glm::vec3(70.0, -5.0, -300));
	model_matrix = glm::scale(model_matrix, glm::vec3(100.0, 0.0, 230.00));
	

}

GLuint Water::getFrameBuffer() {
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	return FBO;
}

void Water::initializeRefractBuffer() {
	refractFBO = getFrameBuffer();
	refractionTexture = frameTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIHGT);
	refractionDepthTexture = frameDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIHGT);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Refraction Framebuffer is not complete!" << endl;

	unBind(displayWidth, displayHeihgt);
}

void Water::initializeReflectBuffer() {
	reflectFBO = getFrameBuffer();
	reflectionTexture = frameTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = depthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;

	unBind(displayWidth, displayHeihgt);
}



void Water::bindRefractionFBO() {
	bindFBO(refractFBO, REFRACTION_WIDTH, REFRACTION_HEIHGT);
}
void Water::bindReflectionFBO() {
	bindFBO(reflectFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void Water::bindFBO(GLuint FBO, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, width, height);
}

void Water::unBind(int width, int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}



GLuint Water::frameTextureAttachment(int width, int height) {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return tex;
}

GLuint Water::depthBufferAttachment(int width, int height) {
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	return depthBuffer;
}

GLuint Water::frameDepthTextureAttachment(int width, int height) {
	GLuint tex;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}



void Water::draw() {

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Water::depthDraw(Shader *s) {

	glBindVertexArray(VAO);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


Water::~Water()
{
	glDeleteFramebuffers(1, &reflectFBO);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractFBO);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
}

void Water::getModel() {
	float y = -1.0;
	vertices.push_back(-1); vertices.push_back(y); vertices.push_back(-1.0);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(-1.0);
	vertices.push_back(-1); vertices.push_back(y); vertices.push_back(1.0);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(1.0);

	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);
}
