#ifndef GAMEOBJECTENVIROMENT_H
#define GAMEOBJECTENVIROMENT_H

#include "GameObject.h"
#include "camera.h"

class GameObjectEnviroment : public GameObject {
public:
	//Attributes of this class
	Texture* texture;
	Shader* shader;
	Vector3 color;
	Matrix44 m;
	//overwritten virtual functions
	void render();
	void update(float dt);
	void setTranslation (float x, float y, float z);
};

#endif