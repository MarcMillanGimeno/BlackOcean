#pragma once

#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include <vector>
#include "shader.h"


class GameObject
{
public:
	GameObject* parent;
	std::vector<GameObject*> children;

	bool is_collider = true;
	Matrix44 modelToCollisions;

	void addChild(GameObject* obj);
	void removeChild(GameObject* obj);
	void clear();//you implement this!

	bool parent_dead = false; 

	bool player = false; 

	Matrix44 getGlobalMatrix(); //returns transform in world coordinates

	//some attributes
	std::string name;
	std::string type = "other";
	Matrix44 model;
	Mesh* mesh;
	float hp = 400;

	//virtual methods which will be overwritten
	virtual void render();
	virtual void update(float elapsed_time);
};
