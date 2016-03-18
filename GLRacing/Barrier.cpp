#include "Barrier.h"
#include "SplineFactory.h"
#include "SOIL.h"


Barrier::Barrier(Shader *s, int position) : Model(s)
{
	this->position = position;
	color = glm::vec3(1.0, 1.0, 1.0);
	material.ambient = glm::vec4(color, 1.0);
	material.diffuse = glm::vec4(glm::vec3(0.7, 0.7, 0.7), 1.0);
	material.specular =glm::vec4( glm::vec3(1.0, 0.8, 0.9), 1.0);
	material.shininess = 4;
	
	getModel();
	setupMesh();
}

glm::vec3 Barrier::getNormal() {
	return glm::vec3(1.0, 0.0, 0.0);
}


Barrier::~Barrier()
{
}

void Barrier::getModel() {
	float y = -0.0001;
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.0);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.5);
	vertices.push_back(-0.2); vertices.push_back(y); vertices.push_back(0.8);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(0.9);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(0.7);
	vertices.push_back(0.1); vertices.push_back(y); vertices.push_back(0.2);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(0.0);
	vertices.push_back(0.5); vertices.push_back(y); vertices.push_back(-0.3);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(-0.5);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(-0.7);
	vertices.push_back(-0.7); vertices.push_back(y); vertices.push_back(-0.3);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(-0.2);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.0);
	vertices.push_back(-0.5); vertices.push_back(y); vertices.push_back(0.5);
	vertices.push_back(-0.2); vertices.push_back(y); vertices.push_back(0.8);
	vertices.push_back(0.2); vertices.push_back(y); vertices.push_back(0.9);

	SplineFactory s;
	vertices = s.buildSpline(vertices);
	vertices = s.extrudeSpline(vertices, 40);
	vector <GLfloat> mesh;
	for (int i = position, size = vertices.size(); i < size; i += 6)
	{
		mesh.push_back(vertices[i] * 500);
		mesh.push_back(vertices[i + 1] * 500);
		mesh.push_back(vertices[i + 2] * 500);

	}
	
	for (int i = 0, size = mesh.size(); i < size; i+=3) {

		glm::vec4 p = glm::vec4(mesh[i], mesh[i + 1], mesh[i + 2], 1.0);
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.0, 1.0, 0.0));
		p = trans * p;
		mesh.push_back(p.x); mesh.push_back(p.y); mesh.push_back(p.z);
	}
	
	vertices = mesh;

	int totalPoints = vertices.size() / 3;
	int depth = (totalPoints / 2);
	int width = (totalPoints / 2);
	for (int i = 0; i < depth - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			indices.push_back(((depth*i) + j) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
		}
	}

	for (int i = 0; i < indices.size(); i += 3)
	{
		int index = indices[i] * 3;
		glm::vec3 v1 = glm::vec3(
			vertices[index],
			vertices[index + 1],
			vertices[index + 2]);
		index = indices[i + 1] * 3;
		glm::vec3 v2 = glm::vec3(
			vertices[index],
			vertices[index + 1],
			vertices[index + 2]);
		index = indices[i + 2] * 3;
		glm::vec3 v3 = glm::vec3(
			vertices[index],
			vertices[index + 1],
			vertices[index + 2]);

		glm::vec3 v1v2 = v2 - v1;
		glm::vec3 v1v3 = v3 - v1;
		glm::vec3 normal = glm::normalize(glm::cross(v1v2, v1v3));
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}

	//Texture
	pData = SOIL_load_image("opengl.jpg", &texture_width, &texture_height, &channels, SOIL_LOAD_RGB);

	if (pData == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "res_texture.png" << ")" << endl;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SOIL_free_image_data(pData);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbi

	for (int i = 0; i < vertices.size() / (6); i += 2)
	{
		
		texCoords.push_back(0.0); texCoords.push_back(0.0);
		texCoords.push_back(0.5); texCoords.push_back(0.0);
		

	}

	for (int i = vertices.size() / (6); i < vertices.size() / 3; i += 2)
	{
		texCoords.push_back(0.0); texCoords.push_back(0.5);
		texCoords.push_back(0.5); texCoords.push_back(0.5);
		
	}

}

void Barrier::draw() {

	glBindVertexArray(VAO);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	this->setMaterialUniform();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Barrier::depthDraw(Shader *s) {

	glBindVertexArray(VAO);
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
