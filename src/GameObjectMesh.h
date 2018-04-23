#ifndef GAMEOBJECTMESH_H
#define GAMEOBJECTMESH_H

#include "GameObject.h"
#include "camera.h"
#include "framework.h"

class GameObjectMesh : public GameObject {
public:
	//Attributes of this class
	Mesh* mesh;
	Texture* texture;
	Texture* texture_spec;
	Shader* shader; 
	std::vector< Vector3 > waypoints;
	int actualPoint = 0;
	std::string type_npc = "default";
	int type_npc_code = -1;
	bool primero = true;

	//Vector3 distance_vec;

	//overwritten virtual functions
	GameObjectMesh();
	void shoot(float dt);
	void render();
	void updateMoving(float dt);
	void update(float dt);
	void NpcNear();
	void lookPlayer(float dt);
	void onCollideBullet(float power);

	double t0, t1;
};

#endif