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
	virtual void processNode(aiNode* node, const aiScene* scene);
	virtual Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	virtual vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	 vector<Texture> textures_loaded;
	string meshDirectory;
private:
	
	
};

