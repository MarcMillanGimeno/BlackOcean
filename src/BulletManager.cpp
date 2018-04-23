#include "BulletManager.h"
#include "managers.h"
#include "GameObjectMesh.h"

//Incializamos el BulletManager
BulletManager::BulletManager(Mesh* theMesh, int max_bullets, Vector3 color_bullet) {
	this->bullets_vector.resize(max_bullets);
	for (int i = 0; i < max_bullets; i++) {
		Bullet new_bullet;
		new_bullet.valid = FALSE;
		new_bullet.bullet_mesh = theMesh;
		this->bullets_vector[i] = new_bullet;
	}
	this->color_default = color_bullet;
}


//Hacemos render malo (Pintamos cada bala)
void BulletManager::renderBad() {
	for (int i = 0; i < this->bullets_vector.size(); i++) {
		if (this->bullets_vector[i].valid) {
			
			Camera* camera = g_scene.camera;

			Matrix44 model;

			model.setScale(1.0f, 1.0f, 1.0f);

			model.traslate(this->bullets_vector[i].position.x, this->bullets_vector[i].position.y, this->bullets_vector[i].position.z);

			Matrix44 mvp = model * camera->viewprojection_matrix;

			//draw
			//enable shader and set uniforms
			this->shader->enable();
			this->shader->setMatrix44("u_mvp", mvp);
			this->shader->setUniform3("color", this->color_default);

			this->bullets_vector[i].bullet_mesh->render(GL_TRIANGLES, shader);
		}
	}
}

//Hacemos render optimizado (Pintamos una sola malla con todas las balas)
void BulletManager::renderOptimized() {
	bool empty = TRUE;
	int count_ind = 0;
	int aux_ind = 0;

	Mesh* all_bullets_mesh = new Mesh();
	all_bullets_mesh->clear();

	Camera* camera = g_scene.camera;

	Matrix44 model;

	//Rellenamos la malla con todas las balas 
	for (int i = 0; i < this->bullets_vector.size(); i++) {
		if (this->bullets_vector[i].valid) {
			empty = FALSE;
			model.scale(this->bullet_scale.x, this->bullet_scale.y, this->bullet_scale.z);
			model.setTranslation(this->bullets_vector[i].position.x, this->bullets_vector[i].position.y, this->bullets_vector[i].position.z);

			for (int j = 0; j < this->bullets_vector[i].bullet_mesh->indices.size(); j++) {
				all_bullets_mesh->indices.push_back(this->bullets_vector[i].bullet_mesh->indices[j] + count_ind);
			}


			for (int j = 0; j < this->bullets_vector[i].bullet_mesh->vertices.size(); j++) {
				all_bullets_mesh->vertices.push_back(model * this->bullets_vector[i].bullet_mesh->vertices[j]);
				aux_ind++;
			}

			count_ind = aux_ind;
		}
	}

	//Si no hay balas en pantalla no pintaremos nada 
	if (!empty) {

		Matrix44 mvp = camera->viewprojection_matrix;

		all_bullets_mesh->genBuffers();

		//draw
		//enable shader and set uniformsa
		this->shader->enable();
		this->shader->setMatrix44("u_mvp", mvp);
		this->shader->setUniform3("color", this->color_default);

		//call render function for mesh, passing shader as param
		//mesh->render() binds the relevant attributes, and drawing code
		all_bullets_mesh->render(GL_TRIANGLES, shader);
	} 
}

//Controlamos el movimiento de cada bala activa
void BulletManager::update(float elapsed_time) {
	float dt = elapsed_time;
	for (int i = 0; i < this->bullets_vector.size(); i++) {
		if (this->bullets_vector[i].valid) {
			Bullet* actual = &bullets_vector[i];
			if (actual->ttl < 0) {
				actual->valid = FALSE;
				break;
			}
			actual->old_position = actual->position;
			actual->position = Vector3(actual->position.x + actual->velocity.x * dt,
				actual->position.y + actual->velocity.y * dt,
				actual->position.z + actual->velocity.z * dt);
			actual->ttl--;
		}
	}

	this->objects.clear();
	
	//Guardamos todos los modelos de colision, para controlar las colisiones
	for (int i = 0; i < g_scene.root->children.size(); i++) {
		if (g_scene.root->children[i]->is_collider) {
			pushCollider(g_scene.root->children[i]);
		}
	};
	g_scene.scene_modificated = false;

	this->checkCollisions(objects);
}

//Rellena las variable de colisiones
void BulletManager::pushCollider(GameObject* actual) {
	actual->modelToCollisions = actual->getGlobalMatrix();

	this->objects.push_back(actual);
	
	if (actual->children.size() != 0) {
		for (int j = 0; j < actual->children.size(); j++) {
			
			if (actual->children[j]->is_collider) {

				pushCollider(actual->children[j]);
			}
		}
	}
}

//Create a Bullet
void BulletManager::createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner, Vector3 color) {
	int valid_pos = -1;

	//Search a valid Bullet Position 
	for (int i = 0; i < bullets_vector.size(); i++) {
		if (!bullets_vector[i].valid) {
			valid_pos = i;
			break;
		}
	}

	if (valid_pos != -1) {
		bullets_vector[valid_pos].color = color;
		bullets_vector[valid_pos].owner = owner;
		bullets_vector[valid_pos].velocity = vel;
		bullets_vector[valid_pos].power = power;
		bullets_vector[valid_pos].ttl = ttl;
		bullets_vector[valid_pos].valid = TRUE;
		bullets_vector[valid_pos].position = pos;
	};
}

//Controla las colisiones de las balas 
void BulletManager::checkCollisions(std::vector<GameObject*>& colliders) {
	//loop all bullets 
	for (size_t i = 0; i < bullets_vector.size(); i++) {
		//Skip current bullet if it's dead
		if (!bullets_vector[i].valid) continue;

		//loop all colliders 
		for (size_t j = 0; j < colliders.size(); j++) {
			//avoid colliding bullet with its parent!
	
			if (bullets_vector[i].owner == colliders[j]) continue;

			//dynamic cast GameObject to GameObjectMesh
			GameObjectMesh* go = dynamic_cast<GameObjectMesh*>(colliders[j]);

			//update collision model matrix (as different gameobjects may use same mesh)
			go->mesh->collision_model->setTransform(go->modelToCollisions.m);

			//test ray collision
			if (go->mesh->collision_model->rayCollision(bullets_vector[i].old_position.v, bullets_vector[i].position.v, false, 0, 1)) {
				if (go->player != bullets_vector[i].owner->player) {
				//destroy bullet
          		bullets_vector[i].valid = false;

				//get collision point, in model space 
				Vector3 collision_point;
				go->mesh->collision_model->getCollisionPoint(collision_point.v, true);

					//call collision function of Gameobject
					go->onCollideBullet(bullets_vector[i].power);
				}

				//skip rest of objects 
				continue;
			}
		}
	}
}


