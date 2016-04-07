#include "WaterFrameBuffers.h"
#include <iostream>
#include "../SOIL.h"
using namespace std;

WaterFrameBuffers::WaterFrameBuffers(int width, int height)
{
	displayWidth = width;
	displayHeihgt = height;
	initializeReflectBuffer();
	initializeRefractBuffer();
	loadDuDvMap();				// DuDv Map for water distortion
}
/*
	Loads water DuDv Map for distortion of the water and makes it a texture
*/
void WaterFrameBuffers::loadDuDvMap() {
	int texture_width, texture_height, channels;
	unsigned char* pData = SOIL_load_image("Assets/Textures/dudvmap.jpg", &texture_width, &texture_height, &channels, SOIL_LOAD_RGBA);
	if (pData == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "res_texture.png" << ")" << endl;

	glGenTextures(1, &dudv);
	glBindTexture(GL_TEXTURE_2D, dudv);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
	SOIL_free_image_data(pData);
	glBindTexture(GL_TEXTURE_2D, 0); 
}

GLuint WaterFrameBuffers::getDuDvMap() {
	return dudv;
}

GLuint WaterFrameBuffers::getRefractionTexture() {
	return refractionTexture;
}

glm::vec4 WaterFrameBuffers::getRefractionClipPlane() {
	return glm::vec4(0, -1, 0, clipHeight);
}

glm::vec4 WaterFrameBuffers::getReflectionClipPlane() {
	return glm::vec4(0, 1, 0, -clipHeight);
}

GLuint WaterFrameBuffers::getReflectionTexture() {
	return reflectionTexture;
}


WaterFrameBuffers::~WaterFrameBuffers()
{
	glDeleteFramebuffers(1, &reflectFBO);
	glDeleteFramebuffers(1, &refractFBO);
}


/*
	Creates a Frame Buffer Object that renders into GL_COLOR_ATTACHMENT0
	and returns its ID
*/
GLuint WaterFrameBuffers::getFrameBuffer() {
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	return FBO;
}

/*
	Initializes the Refraction Frame buffer object
*/
void WaterFrameBuffers::initializeRefractBuffer() {
	refractFBO = getFrameBuffer();
	refractionTexture = frameTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIHGT);
	//refractionDepthTexture = frameDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIHGT);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	unBind(displayWidth, displayHeihgt);
}


/*
	Initializes the Reflection Frame buffer object
*/
void WaterFrameBuffers::initializeReflectBuffer() {
	reflectFBO = getFrameBuffer();
	reflectionTexture = frameTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	//reflectionDepthBuffer = depthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	unBind(displayWidth, displayHeihgt);
}



void WaterFrameBuffers::bindRefractionFBO() {
	bindFBO(refractFBO, REFRACTION_WIDTH, REFRACTION_HEIHGT);
}
void WaterFrameBuffers::bindReflectionFBO() {
	bindFBO(reflectFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

GLuint WaterFrameBuffers::getRefractionFBO() {
	return refractFBO;
}

GLuint WaterFrameBuffers::getReflectionFBO() {
	return reflectFBO;
}

void WaterFrameBuffers::bindFBO(GLuint FBO, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, width, height);
}

void WaterFrameBuffers::unBind(int width, int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}



GLuint WaterFrameBuffers::frameTextureAttachment(int width, int height) {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

GLuint WaterFrameBuffers::depthBufferAttachment(int width, int height) {
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);


	return depthBuffer;
}

GLuint WaterFrameBuffers::frameDepthTextureAttachment(int width, int height) {
	GLuint tex;
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex, 0);

	return tex;
}
