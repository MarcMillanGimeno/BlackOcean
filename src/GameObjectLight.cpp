#include "GameObjectLight.h"

GameObjectLight::GameObjectLight (float x, float y, float z){
	this->light_pos = Vector3(x, y, z);
}

void GameObjectLight::render() {}
void GameObjectLight::update(float dt) {}