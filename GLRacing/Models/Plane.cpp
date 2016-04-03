#include "Plane.h"
#include "../SOIL.h"



Plane::Plane(Shader *s) : Model(s)
{
	color = glm::vec3(0.2, 0.5, 0.2);
	material.ambient = glm::vec4(color, 1.0);
	material.diffuse = glm::vec4(0.2, 0.6, 0.2, 1.0);
	material.specular = glm::vec4(0.0, 0.0, 0.0, 1.0);
	material.shininess = 1;
	getModel();
	setupMesh();
	
	model_matrix = glm::translate(model_matrix, glm::vec3(-offsetX, -0.1, -offsetZ));
	model_matrix = glm::scale(model_matrix, glm::vec3(SCALE, SCALE*3, SCALE));
}

float Plane::getHeight(float modelX, float modelZ) {
	float x = (modelX + offsetX) / SCALE, z = (modelZ + offsetZ) / SCALE;
	pair<float, glm::vec3> result;
	float gridSize = TERRAIN_WIDTH;
	int gridX = (int)floor(x);
	int gridZ = (int)floor(z);
	if (gridX > TERRAIN_WIDTH || gridZ > TERRAIN_DEPTH ||
		gridX < 0 || gridZ < 0) {
		return 0.0;
	}
	float xCoord = x / gridSize;
	float zCoord = z / gridSize;
	float answer;
	if (xCoord <= (1 - zCoord)) {
		answer = findHeight(glm::vec3(0, heights[gridX][gridZ], 0), glm::vec3(1,
				heights[gridX + 1][gridZ], 0), glm::vec3(0,
					heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}
	else {
		answer = findHeight(glm::vec3(1, heights[gridX + 1][gridZ], 0), glm::vec3(1,
				heights[gridX + 1][gridZ + 1], 1), glm::vec3(0,
					heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}
	result.first = answer*SCALE*3;

	return result.first;
}

float Plane::findHeight(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}


Plane::~Plane()
{

}

glm::vec3 Plane::getColor() {
	return color;
}

void Plane::draw() {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform3f(shaderProgram->getUniform("vertex_color"), color.x, color.y, color.z);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	setMaterialUniform();
	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Plane::depthDraw(Shader *s) {
	glBindVertexArray(VAO);
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Plane::getModel() {

	pData = SOIL_load_image("Assets/HeightMap/terrain2.tga", &texture_width, &texture_height, &channels, SOIL_LOAD_L);



	for (int j = 0; j<texture_height-1; j++) {
		for (int i = 0; i<texture_width-1; i++) {
			vertices.push_back(float(i));
			vertices.push_back((float)((((float) pData[i + (texture_width*j)])-128)/128)*2);
			vertices.push_back(float(j));
			heights[i][j] = (float)((((float)pData[i + (texture_width*j)]) - 128) / 128) * 2;
		}
	}

	SOIL_free_image_data(pData);

	int totalPoints = vertices.size()/3;
	int depth = (texture_height-1) ;
	int width = (texture_width-1);
	for (int i = 0; i < depth-1; i++)
	{
		for (int j = 0; j < width-1; j++)
		{
			indices.push_back(((depth*i) + j) );
			indices.push_back(((depth*i) + j + 1) );
			indices.push_back(((depth*i) + j + depth));
			indices.push_back(((depth*i) + j + 1) );
			indices.push_back(((depth*i) + j + depth + 1));
			indices.push_back(((depth*i) + j + depth));
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
	pData = SOIL_load_image("Assets/Textures/grass.jpg", &texture_width, &texture_height, &channels, SOIL_LOAD_RGB);

	if (pData == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "res_texture.png" << ")" << endl;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SOIL_free_image_data(pData);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbi

	for (int i = 0; i <= sqrt(vertices.size()/3); i++)
	{
		for (int j = 0; j <= sqrt(vertices.size()/3); j +=2)
		{
			texCoords.push_back(0.0); texCoords.push_back(0.0);
			texCoords.push_back(1.0); texCoords.push_back(0.0);

		}

		for (int j = 0; j <= sqrt(vertices.size()/3); j +=2)
		{
			texCoords.push_back(0.0); texCoords.push_back(1.0);
			texCoords.push_back(1.0); texCoords.push_back(1.0);

		}

	}

}
