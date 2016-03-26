#pragma once
#include "ImportedModel.h"

class Bridge :
	public ImportedModel
{
public:
	Bridge(Shader *s);
	~Bridge();
	void depthDraw(Shader *s);
	void  draw();
	glm::vec4 position;
	float getHeight(float modelX, float modelY);

private:
	float collisionRadius;
	glm::vec4 collisionCenter;
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

