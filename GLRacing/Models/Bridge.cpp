#include "Bridge.h"



Bridge::Bridge(Shader *s) : ImportedModel(s)
{
	material.ambient = glm::vec4(1, 1, 1, 1.0);
	material.diffuse = glm::vec4(1, 1, 1, 1.0);
	material.specular = glm::vec4(1, 1, 1, 1.0);
	material.shininess = 76.8;

	getModel("Assets/bridge/bridge.obj ");
	

	model_matrix = glm::translate(model_matrix, glm::vec3(95.0, 3.2, -253.0));
	position = model_matrix * glm::vec4(0.0 ,0.0, 0.0, 1.0);

	//Set collision Sphere
	collisionRadius = 240.45f;
	collisionCenter = model_matrix * glm::vec4(0, -236.5, 0, 1.0);	
}


/*
	Get Current height of collision sphere
*/
float Bridge::getHeight(float modelX, float modelZ) {
	if (modelX > collisionCenter.x + collisionRadius || modelX < collisionCenter.x - collisionRadius ||
		modelZ > collisionCenter.z + collisionRadius || modelZ < collisionCenter.z - collisionRadius) {

		return -99999;
	}
	float x = collisionCenter.x - modelX;
	float z = collisionCenter.z - modelZ;
	float y2 = (collisionRadius*collisionRadius) - (x * x) - (z*z);
	float y = sqrt(y2) + collisionCenter.y;
	return (!isnan(y))? y : -99999;

}


Bridge::~Bridge()
{
}



