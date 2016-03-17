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
	model_matrix = glm::scale(model_matrix, glm::vec3(2.5));
}


Plane::~Plane()
{
}

glm::vec3 Plane::getColor() {
	return color;
}

void Plane::draw() {
	

	glBindVertexArray(VAO);

	//glUniform1i(shaderProgram->getUniform("heightMapTexture"), 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	//
	//
	//glUniform1i(shaderProgram->getUniform("normalMapTexture"), 1);
	//
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, NormalMap);
	//
	//
	//
	//glUniform2i(shaderProgram->getUniform("HALF_TERRAIN_SIZE"), TERRAIN_WIDTH >> 1, TERRAIN_DEPTH >> 1);
	//glUniform1f(shaderProgram->getUniform("scale"), 10);
	//glUniform1f(shaderProgram->getUniform("half_scale"), 6);

	glUniform3f(shaderProgram->getUniform("vertex_color"), color.x, color.y, color.z);
	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	setMaterialUniform();
	//glUniformMatrix4fv(shaderProgram->getUniform("view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void Plane::getModel() {

	pData = SOIL_load_image("terrain2.tga", &texture_width, &texture_height, &channels, SOIL_LOAD_L);



	for (int j = 0; j<texture_height; j++) {
		for (int i = 0; i<texture_width; i++) {
			vertices.push_back(float(i));
			vertices.push_back((float)((((float) pData[i + (texture_width*j)])-128)/128)*2);
			//cout << (float) pData[j] << " -> " << (((float)pData[j]) - 128) / 128 << endl;
			vertices.push_back(float(j));
		}
	}
	//for (int i = 0; i < texture_width*texture_height -1; i++)
	//{
	//	cout << (unsigned int) pData[i] << "\t" << (unsigned int)pData[i + 1] << "\t" << (unsigned int)pData[i + 2] << endl;
	//	system("pause");
	//}

	//for (int i = 0; i < texture_height*texture_width; i++) {
	//	cout << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << endl;
	//}

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

}

void Plane::setupMesh() {


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	if (!indices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &(indices[0]), GL_STATIC_DRAW);
	}
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
