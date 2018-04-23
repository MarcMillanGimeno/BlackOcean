#ifndef GAMEOBJECTLIGHT_H
#define GAMEOBJECTLIGHT_H

#include "GameObject.h"

class GameObjectLight : public GameObject {
public:
	GameObjectLight(float x, float y, float z);
	Vector3 light_pos;
	void render();
	void update(float dt);
};
#endif