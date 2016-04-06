#pragma once
#include "Model.h"
#include "Mesh.h"


class ImportedModel :
	public Model
{
public:
	ImportedModel(Shader *s);
	ImportedModel(Shader *s, string objFile, glm::vec3 trans, glm::vec3 scale, float rot);
	~ImportedModel();
	void draw();
	void depthDraw(Shader *s);
	void setPosition(glm::vec3 trans, glm::vec3 scale, float rot);

protected:
	void getModel(string path);
	vector<Mesh> meshes;
	virtual void processNode(aiNode* node, const aiScene* scene);
	virtual Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	virtual vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	 vector<Texture> textures_loaded;
	string meshDirectory;
private:
	string texFile;
	
	
};

