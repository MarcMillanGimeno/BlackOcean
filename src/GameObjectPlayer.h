#ifndef GAMEOBJECTPLAYER_H
#define GAMEOBJECTPLAYER_H

#include "GameObjectMesh.h"
#include "camera.h"

class GameObjectPlayer : public GameObjectMesh {
public:
	int i = 0;
	//Attributes of this class
	Texture* texture;
	Texture* texture_spec;
	Shader* shader;
	Shader* shader_line;
	float pitch_speed = 10;
	float roll_speed = 10;

	int aux = 0;

	//overwritten virtual functions
	void render();
	void update(float dt);
};

#endif