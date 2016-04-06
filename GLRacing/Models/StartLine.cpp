#include "StartLine.h"



StartLine::StartLine(Shader *s) : ImportedModel(s)
{
	material.ambient = glm::vec4(1, 1, 1, 1.0);
	material.diffuse = glm::vec4(1, 1, 1, 1.0);
	material.specular = glm::vec4(1, 1, 1, 1.0);
	material.shininess = 76.8;

	getModel("Assets/Startline/violet_car.obj");


	model_matrix = glm::translate(model_matrix, glm::vec3(-250.0, 0.0, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(2));
}


StartLine::~StartLine()
{
}

