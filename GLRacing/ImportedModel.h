#pragma once
#include "Model.h"
#include "Mesh.h"


class ImportedModel :
	public Model
{
public:
	ImportedModel(Shader *s);
	~ImportedModel();

protected:
	void getModel(string path);
	vector<Mesh> meshes;
private:
	
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	vector<Texture> textures_loaded;
	string meshDirectory;
};

