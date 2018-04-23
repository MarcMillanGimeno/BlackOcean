
#ifndef AABB_H
#define AABB_H

#include <vector>
#include "framework.h"
#include "mesh.h"

class AABB {
	public: 
		Vector3 min; 
		Vector3 max;
		Vector3 center; 

		Mesh* mesh_box;

		AABB();
		void setCenter();
		void collisionBox();
		void drawCollisionModel();
};

#endif 