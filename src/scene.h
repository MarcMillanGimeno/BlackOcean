#pragma once

#include "GameObject.h"
#include "camera.h" 
#include "GameObjectLight.h" 
#include "GameObjectPlayer.h" 
#include "BulletManager.h"
#include "Menu.h"
#include "HUD.h"

class Scene
{
public:
	Scene();
	~Scene();
	bool debugg_camera = false;
	//scene graph starts from here
	GameObject* root;
	GameObjectPlayer* player;
	//the camera used to render the scene
	Camera* camera;
	Camera* camera_aux;
	float sensibilidad = 0.001f;
	Shader* aabb_shader;
	int state = 0;
	Menu* menu;
	HUD* hud;

	int enemies = 0;

	bool scene_modificated = true;

	//bullets control
	BulletManager* g_bulletManager;
	BulletManager* g_bulletEnemyManager;

	std::vector<GameObject*> all_enemies;

	//a list of all lights - could be within graph or not
	std::vector<GameObjectLight*> lights;
	//standard render and update functions
	void render();
	void update(float dt);
	void addPlayer(GameObjectPlayer* player);
	void getAllEnemies(std::vector<GameObject*> & enemies, GameObject* current);
};

