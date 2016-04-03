#pragma once
#include "../Models/Model.h"
class Water :
	public Model
{
public:
	Water(Shader *s, int displayWidth, int displayHeihgt);
	~Water();
	void draw();
	void depthDraw(Shader *s);
	void bindRefractionFBO();
	void bindReflectionFBO();
	GLuint getRefractionTexture();
	GLuint getReflectionTexture();
	void unBind(int width, int height);

private:
	void getModel();
	GLuint refractFBO, refractionTexture, refractionDepthTexture, refractionDepthBuffer,
		reflectFBO, reflectionTexture, reflectionDepthTexture, reflectionDepthBuffer;
	GLuint getFrameBuffer();
	void initializeRefractBuffer();
	void initializeReflectBuffer();
	GLuint frameTextureAttachment(int width, int height);
	GLuint frameDepthTextureAttachment(int width, int height);
	GLuint depthBufferAttachment(int width, int height);
	void bindFBO(GLuint FBO, int width, int height);
	int displayWidth, displayHeihgt;


	static const int REFLECTION_WIDTH = 320, REFLECTION_HEIGHT = 180;
	static const int REFRACTION_WIDTH = 1280, REFRACTION_HEIHGT = 720;
};

