#pragma once
#include "Model.h"
#include "BillBoardModel.h"


class Fence : public Model {
public:
	Fence(Shader *s, glm::vec3 trans, float scale);
	~Fence();
	void  draw();
private:
	void getModel();
};