#pragma once

#include "bullets.h"

class BulletManager {
	public:
		std::vector<Bullet> bullets_vector; //an array of all bullets
		std::vector<Bullet> bullets_enemy_vector;
		Shader* shader; //the shader used to draw the bullet
		bool can_fire = true; //optional - use this to control fire rate

		Vector3 bullet_scale = Vector3(1.0f, 1.0f, 1.0f); //scale of the bullet
		Vector3 color_default; //Color of the bullet

		std::vector<GameObject*> objects; //Variable que almacena las colisiones de las balas

		BulletManager(Mesh* theMesh, int max_bullets, Vector3 color_bullet); //constructor

		void renderOptimized(); //draw all bullets in array 
		void renderFast(); //draw all bullets in array 
		void renderBad(); //draw all bullets one to one 
		void update(float elapsed_time); //update position of all bullets
		void createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner, Vector3 color); //Create a bullet
		void checkCollisions(std::vector<GameObject*> & colliders); //Check collisions 
		void pushCollider(GameObject* actual); //Add to objects a collision of a bullets
};