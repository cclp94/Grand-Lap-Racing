#pragma once
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm\vec4.hpp>

class WaterFrameBuffers
{
public:
	WaterFrameBuffers(int width, int height);
	~WaterFrameBuffers();
	void bindRefractionFBO();
	void bindReflectionFBO();
	GLuint getRefractionTexture();
	GLuint getReflectionTexture();
	void unBind(int width, int height);
	GLuint getRefractionFBO();
	GLuint getReflectionFBO();
	void setClipHeight(float height) {
		clipHeight = height;
	}
	glm::vec4 getRefractionClipPlane();
	glm::vec4 getReflectionClipPlane();
	GLuint getDuDvMap();
	static const int REFLECTION_WIDTH = 320, REFLECTION_HEIGHT = 180;
	static const int REFRACTION_WIDTH = 1280, REFRACTION_HEIHGT = 720;

private:
	GLuint refractFBO, refractionTexture, refractionDepthTexture,
		reflectFBO, reflectionTexture, reflectionDepthBuffer;
	GLuint dudv;
	GLuint getFrameBuffer();
	void initializeRefractBuffer();
	void initializeReflectBuffer();
	GLuint frameTextureAttachment(int width, int height);
	GLuint frameDepthTextureAttachment(int width, int height);
	GLuint depthBufferAttachment(int width, int height);
	void bindFBO(GLuint FBO, int width, int height);
	int displayWidth, displayHeihgt;
	float clipHeight;
	void loadDuDvMap();
	
};

