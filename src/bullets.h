#pragma once

#include "GameObject.h"

struct Bullet
{
	Vector3 color; //bullet color
	Mesh* bullet_mesh; //the mesh to render the bullet
	bool valid; //whether it's 'alive' or not
	Vector3 position; //current position
	Vector3 old_position; //store old pos before updating
	Vector3 velocity; //movement vector
	float ttl; //time-to-live
	float power; //how much damage bullet does
	GameObject* owner; //object that fired bullet
};