#include "Road.h"
#include "../SplineFactory.h"
#include "../SOIL.h"


Road::Road(Shader *s, Plane *terrain) : Model(s)
{
		color = glm::vec3(0.3, 0.3, 0.3);
		material.ambient = glm::vec4(0.5, 0.5, 0.5, 1.0);
		material.diffuse = glm::vec4(0.8, 0.8, 0.8, 1.0);
		material.specular = glm::vec4(0.0, 0.0, 0.0, 1.0);
		material.shininess = 1;
		getModel(terrain);
		setupMesh();
		model_matrix = glm::scale(model_matrix, glm::vec3(500.00, 0.0, 500.00));
}

bool Road::checkBound(float x, float z) {
	float previousX, previousZ;
	for (int i = 0; i < (vertices.size()/3)-1; i++)
	{
		int location = i * 3;
		glm::vec4 position = model_matrix * glm::vec4(vertices[location], 0.0, vertices[location + 2], 1.0);
		previousX = position.x;
		previousZ = position.z;
		
		position = model_matrix * glm::vec4(vertices[location+3], 0.0, vertices[location + 5], 1.0);
		float roadX = position.x;
		float roadZ = position.z;

		if (previousZ < z&& roadZ > z) {
			if (previousX > x && roadX < x) {
				return true;
			}
			else if (previousX < x && roadX > x) {
				return true;
			}
		}
		else if (previousZ > z && roadZ < z) {
			if (previousX > x && roadX < x) {
				return true;
			}
			else if (previousX < x && roadX > x) {
				return true;
			}
		}
		if (previousX > x && roadX < x) {
			if (previousZ < z && roadZ > z) {
				return true;
			}else if (previousZ > z && roadZ < z) {
				return true;
			}
		}
		else if (previousX < x && roadX > x) {
			if (previousZ < z && roadZ > z) {
				return true;
			}
			else if (previousZ > z && roadZ < z) {
				return true;
			}
		}

	}
	return false;
}


Road::~Road()
{
}

void Road::getModel(Plane *terrain) {
	float y = 0.3;
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

	//SplineFactory s;
	vertices = SplineFactory::buildSpline(vertices);
	vertices = SplineFactory::extrudeSpline(vertices, 40);


	pData = SOIL_load_image("Assets/Textures/tunnel_road.jpg", &texture_width, &texture_height, &channels, SOIL_LOAD_RGB);

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

	for (int i = 0; i < vertices.size()/12; i++)
	{
		texCoords.push_back(0.0); texCoords.push_back(0.0);
		texCoords.push_back(1.0); texCoords.push_back(0.0);
		texCoords.push_back(0.0); texCoords.push_back(1.0);
		texCoords.push_back(1.0); texCoords.push_back(1.0);
	}
	


	normals.push_back(0.0); normals.push_back(1.0); normals.push_back(0.0);
}

void Road::draw() {

	glBindVertexArray(VAO);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniformMatrix4fv(shaderProgram->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	this->setMaterialUniform();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() /3);

	glBindVertexArray(0);
}

void Road::depthDraw(Shader *s) {

	glBindVertexArray(VAO);
	glUniformMatrix4fv(s->getUniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);

	glBindVertexArray(0);
}


