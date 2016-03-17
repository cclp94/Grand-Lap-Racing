#include "Plane.h"
#include <SOIL\SOIL.h>



Plane::Plane(Shader *s) : Model(s)
{
	color = glm::vec3(0.2, 0.5, 0.2);
	material.ambient = glm::vec4(color, 1.0);
	material.diffuse = glm::vec4(0.5, 0.6, 0.2, 1.0);
	material.specular = glm::vec4(0.7, 0.5, 0.2, 1.0);
	material.shininess = 1;
	getModel();
	setupMesh();
	
	model_matrix = glm::translate(model_matrix, glm::vec3(-500, -1.0, -500.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(10));
}


Plane::~Plane()
{
}

glm::vec3 Plane::getColor() {
	return color;
}

void Plane::draw() {
	glBindVertexArray(VAO);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glUniform3f(shaderProgram->getUniform("vertex_color"), color.x, color.y, color.z);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	setMaterialUniform();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void Plane::depthDraw(Shader *s) {
	glBindVertexArray(VAO);
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Plane::getModel() {

	pData = SOIL_load_image("terrain2.tga", &texture_width, &texture_height, &channels, SOIL_LOAD_L);



	for (int j = 0; j<texture_height; j++) {
		for (int i = 0; i<texture_width; i++) {
			vertices.push_back(float(i));
			vertices.push_back((float)((((float) pData[i + (texture_width*j)])-128)/128)*2);
			vertices.push_back(float(j));
		}
	}

	SOIL_free_image_data(pData);

	int totalPoints = vertices.size()/3;
	int depth = (texture_height) ;
	int width = (texture_width);
	for (int i = 0; i < depth -1; i++)
	{
		for (int j = 0; j < width -1; j++)
		{
			indices.push_back(((depth*i) + j) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
			indices.push_back(((depth*i) + j + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth + 1) % totalPoints);
			indices.push_back(((depth*i) + j + depth) % totalPoints);
		}
	}
	//getNormal();

	for (int i = 0; i < indices.size(); i+=3)
	{
		int index = indices[i] * 3;
		glm::vec3 v1 = glm::vec3(
			vertices[index],
			vertices[index+1],
			vertices[index+2]);
		index = indices[i+1] * 3;
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
	pData = SOIL_load_image("grass.jpg", &texture_width, &texture_height, &channels, SOIL_LOAD_RGB);

	if (pData == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "res_texture.png" << ")" << endl;

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SOIL_free_image_data(pData);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbi

	for (int i = 0; i < vertices.size() / 12; i++)
	{
		texCoords.push_back(0.0); texCoords.push_back(0.0);
		texCoords.push_back(1.0); texCoords.push_back(0.0);
		texCoords.push_back(0.0); texCoords.push_back(1.0);
		texCoords.push_back(1.0); texCoords.push_back(1.0);
	}

}
