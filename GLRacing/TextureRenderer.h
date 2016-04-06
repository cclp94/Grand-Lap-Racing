#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class TextureRenderer
{
public:
	TextureRenderer(int width, int heihgt);
	~TextureRenderer();
	void bind();
	void unbind(int width, int height);
	GLuint getTexture();
	static const int TEX_WIDTH = 526, TEX_HEIGHT = 526;

private:
	GLuint FBO, texture;
	void initializeFrameBuffer();
	GLuint frameTextureAttachment(int width, int height);
	int displayWidth, displayHeihgt;
	
};

