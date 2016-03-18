#include "kart.h"
#include "Camera.h"

kart::~kart() {
	delete camera;
}

glm::vec3 kart::getCameraPosition() {
	return camera->cameraPos;
}



kart::kart(Shader *s) : Model(s){
	color = glm::vec3(0.3, 0.1, 0.2);
	material.ambient = glm::vec4(1, 1, 1, 1.0);
	material.diffuse = glm::vec4(1, 1, 1,1.0);
	material.specular = glm::vec4(1, 1, 1, 1.0);
	material.shininess = 76.8;
	position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	maxSpeed = 0.5f;
	acceleration = 0.05f;
	speed = 0.0f;
	isAccelarating = false;
	getModel();
	//setupMesh();
	camera = new Camera();
	camera->cameraPos = glm::vec3(position.x, position.y + 2, position.z + 5.0);

	model_matrix = glm::translate(model_matrix, glm::vec3(-250.0, 0.0, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(0.7));
	model_matrix = glm::rotate(model_matrix,(float) glm::radians(180.0), glm::vec3(0.0, 1.0, 0.0));
}

void kart::move() {
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, getSpeed()));
	glm::vec4 previousPos = position;
	position = model_matrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	camera->cameraPos = glm::vec3(model_matrix * glm::vec4(0.0f, 5.0f, -15.0f, 1.0f));

	update();
}

void kart::turn(float angle) {
	if (speed != 0) {
		model_matrix = glm::rotate(model_matrix, angle, glm::vec3(0.0, 1.0, 0.0));
	}
}

glm::mat4 kart::getCameraView() {
	view_matrix = glm::lookAt(camera->cameraPos,glm::vec3(position),camera->cameraUp);
	return view_matrix;
}

void kart::notAccelerating() {
	isAccelarating = false;
}

void kart::accelerating() {
	isAccelarating = true;
}

void kart::accelerate() {
	accelerating();
	isReverse = false;
	if (speed == maxSpeed) {
		speed = maxSpeed;
	}
	else {
		speed += acceleration;
	}
}

void kart::deaccelerate() {
	accelerating();
	isReverse = true;
	if (speed == -maxSpeed) {
		speed = -maxSpeed;
	}
	else {
		speed -= acceleration * 2;		// Breaking faster than acceleration
	}
}

float kart::getSpeed() {
	return speed / 100;
}

void kart::update() {
	if (!isAccelarating) {
		if (speed < 0) {
			speed += 0.1;
		}
		else if (speed > 0) {
			speed -= 0.1;
		}
		if (speed < 0.1 && speed > -0.1)
			speed = 0;
	}
	else {
		if (!isReverse) {
			accelerate();
		}
		else
			deaccelerate();
	}
}

glm::vec3 kart::getColor() {
	return color;
}

void kart::draw() {
	move();
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform3f(shaderProgram->getUniform("viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
	//this->setMaterialUniform();
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}

void kart::depthDraw(Shader *s) {
	move();
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shaderProgram);
}


void kart::getModel() {

	//vertices.push_back(0.5);	vertices.push_back(0.5);	vertices.push_back(0.5);
	//vertices.push_back(-0.5f);	vertices.push_back(0.5);	vertices.push_back(0.5);
	//vertices.push_back(0.3);	vertices.push_back(0.3);	vertices.push_back(-0.5f);
	//vertices.push_back(-0.3f);	vertices.push_back(0.3);	vertices.push_back(-0.5f);
	//vertices.push_back(0.5);	vertices.push_back(0.0f);	vertices.push_back(0.5);
	//vertices.push_back(-0.5f);	vertices.push_back(0.0f);	vertices.push_back(0.5);
	//vertices.push_back(-0.3f);	vertices.push_back(0.1f);	vertices.push_back(-0.5f);
	//vertices.push_back(0.3);	vertices.push_back(0.1f);	vertices.push_back(-0.5f);
	//
	//
	//indices.push_back(3); indices.push_back(2);indices.push_back(6);
	//indices.push_back(7); indices.push_back(4); indices.push_back(2);
	//indices.push_back(0); indices.push_back(3); indices.push_back(1);
	//indices.push_back(6); indices.push_back(5); indices.push_back(4);
	//indices.push_back(1); indices.push_back(0);

	//================================================================================

	Assimp::Importer import;
	string path = "Assets/Enzo\'s Car/enzo_car.obj";
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}

	this->meshDirectory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);


}

void kart::processNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}


Mesh kart::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		if (mesh->mNormals != NULL) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
		}
		else {
			vector.x = 0.0;
			vector.y = 0.0;
			vector.z = 1.0;
			
		}
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = this->loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> kart::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = Mesh::TextureFromFile(str.C_Str(), this->meshDirectory);
		texture.type = typeName;
		texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}

