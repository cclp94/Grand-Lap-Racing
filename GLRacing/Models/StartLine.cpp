#include "StartLine.h"



StartLine::StartLine(Shader *s) : ImportedModel(s)
{
	material.ambient = glm::vec4(1, 1, 1, 1.0);
	material.diffuse = glm::vec4(1, 1, 1, 1.0);
	material.specular = glm::vec4(1, 1, 1, 1.0);
	material.shininess = 76.8;

	getModel("Assets/Startline/violet_car.obj");


	model_matrix = glm::translate(model_matrix, glm::vec3(-250.0, 0.0, 0.0));
	//model_matrix = glm::scale(model_matrix, glm::vec3(0.2, 0.2, 0.2));
}


StartLine::~StartLine()
{
}


vector<Texture> StartLine::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	aiString str;
	Texture texture;
	texture.id = Mesh::TextureFromFile("rubberTex.jpg", this->meshDirectory);
	texture.type = typeName;
	texture.path = str;
	textures.push_back(texture);
	return textures;
}
