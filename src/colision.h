#ifndef COLISION_H
#define COLISION_H

#include <vector>
#include "framework.h"

class Colision {
public:
	std::vector<Vector3> vertices;
	
	float max_x;
	float min_x;
	float max_y;
	float min_y;
	float max_z;
	float min_z;

	int intersection = 0;

	Colision();
	void checkVolum(Vector3 vertex); 
	void update(Matrix44 model);
};

#endif