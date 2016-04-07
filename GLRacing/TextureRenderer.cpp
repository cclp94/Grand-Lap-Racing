#include "TextureRenderer.h"

#include <iostream>


TextureRenderer::TextureRenderer(int width, int heihgt)
{
	displayWidth = width;
	heihgt = heihgt;
	initializeFrameBuffer();
}


TextureRenderer::~TextureRenderer()
{
	glDeleteFramebuffers(1, &FBO);	// delete FBO
}

/*
	Initialize FBO with 1 Color attachment for rendering
*/
void TextureRenderer::initializeFrameBuffer() {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	texture = frameTextureAttachment(TEX_WIDTH, TEX_HEIGHT);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	unbind(displayWidth, displayHeihgt);
}


/*
	@return texture ID
*/
GLuint TextureRenderer::getTexture() {
	return texture;
}


/*
	Generates a texture  and binds it to the framebuffer object
*/
GLuint TextureRenderer::frameTextureAttachment(int width, int height) {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}
/*
	Unbinds currently bound FBO
*/
void TextureRenderer::unbind(int width, int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

/*
	Binds FBO
*/
void TextureRenderer::bind() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, TEX_WIDTH, TEX_HEIGHT);
}
