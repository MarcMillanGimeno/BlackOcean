#include "GameObjectMesh.h"
#include "managers.h"
#include "utils.h"
#include "game.h"

GameObjectMesh::GameObjectMesh() {
	waypoints.push_back(Vector3(30, 0, 30));
	waypoints.push_back(Vector3(30, 0, -30));
	waypoints.push_back(Vector3(-30, 0, -30));
	waypoints.push_back(Vector3(-30, 0, 30));
}

void GameObjectMesh::render() {
	Camera* camera = g_scene.camera;
	Vector3 l_pos = g_scene.lights[0]->light_pos;
	//Vector3 l_pos(100.0f, 100.0f, 100.0f);
	//build a rotation matrix - angle is incremented in update()
	//create our mvp
	Matrix44 mvp = this->getGlobalMatrix() * camera->viewprojection_matrix;

	//this->mesh->aabb.center = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);

	//create normal matrix
	Matrix44 inverse_m = model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();


	//enable shader and set uniformsa
	this->shader->enable();
	this->shader->setMatrix44("u_normal_matrix", normal_m);
	this->shader->setMatrix44("u_mvp", mvp);
	this->shader->setUniform3("u_light", l_pos);
	this->shader->setUniform3("u_camera_pos", camera->eye);
	this->shader->setTexture("u_texture_diffuse", this->texture);	this->shader->setTexture("u_texture_specular", this->texture_spec);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->mesh->render(GL_TRIANGLES, shader);

	//disable the shader
	this->shader->disable();

	g_scene.aabb_shader->enable();
	//mvp = camera->viewprojection_matrix;
	g_scene.aabb_shader->setMatrix44("u_mvp", mvp);
	this->mesh->renderAABB(g_scene.aabb_shader);
	g_scene.aabb_shader->disable();
	
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
}

void GameObjectMesh::update(float dt) {
	if (this->parent_dead) {
		this->type_npc_code = 1;
		this->type_npc = "static_shoot";
	}
	switch (this->type_npc_code)
	{
	case 0:
		this->NpcNear();
		break;

	//STATIC_TARGET
	case 1:
		this->lookPlayer(dt);
		this->shoot(dt);
		//this->perseguir(dt);
		//std::cout << "AQUI" << std::endl;

		break;
	
	//MOVING
	case 2:
		this->updateMoving(dt);
		this->NpcNear();
		break;

	case 3:
		break;
	case 4:
		this->shoot(dt);
		break;
	
	default:
		break;
	}

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}
	
}


void GameObjectMesh::shoot(float dt) {
	Vector3 f = this->getGlobalMatrix().frontVector().normalize();
	Vector3 p = Vector3(this->getGlobalMatrix().m[12], this->getGlobalMatrix().m[13], this->getGlobalMatrix().m[14]);
	Vector3 u = Vector3(this->getGlobalMatrix().m[4], this->getGlobalMatrix().m[5], this->getGlobalMatrix().m[6]).normalize();

	if (primero) {
		primero = false;
		t0 = clock();
	}
	//if (aux == 0){
	t1 = clock();
	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	if (time > 0.2) {
		g_scene.g_bulletEnemyManager->createBullet(p, Vector3(-f.x * 300.0, -f.y * 300.0, -f.z * 300.0), 150, 400, this, Vector3(1.0f, 0.0f, 0.0f));
		t0 = t1;
	}

}

void GameObjectMesh::NpcNear() {
	//Vector3 player_pos = g_scene.player_pos;
	Vector3 pos_pj = Vector3(g_scene.player->model.m[12], g_scene.player->model.m[13], g_scene.player->model.m[14]);
	Vector3 this_pos = Vector3(this->getGlobalMatrix().m[12], this->getGlobalMatrix().m[13], this->getGlobalMatrix().m[14]);
	//std::cout << this_pos.x << " " << this_pos.y << " " << this_pos.z << std::endl;

	double distance = sqrt(pow(pos_pj.x - this_pos.x, 2) + pow(pos_pj.y - this_pos.x, 2) + pow(pos_pj.z - this_pos.z, 2));
	//distance_vec = Vector3(pos_pj.x - this_pos.x, pos_pj.y - this_pos.y, pos_pj.z - this_pos.z);
	//std::cout << distance_vec.x << " " << distance_vec.y << " " << distance_vec.z << std::endl;

	if (distance < 1400) {
		this->type_npc_code = 1;
		this->type_npc = "static_shoot";
	}
}

void GameObjectMesh::lookPlayer(float dt) {
	Vector3 pos_pj = Vector3(g_scene.player->model.m[12], g_scene.player->model.m[13], g_scene.player->model.m[14]);
	Vector3 this_pos = Vector3(this->getGlobalMatrix().m[12], this->getGlobalMatrix().m[13], this->getGlobalMatrix().m[14]);
	//std::cout << this_pos.x << " " << this_pos.y << " " << this_pos.z << std::endl;

	double distance = sqrt(pow(pos_pj.x - this_pos.x, 2) + pow(pos_pj.y - this_pos.x, 2) + pow(pos_pj.z - this_pos.z, 2));
	//distance_vec = Vector3(pos_pj.x - this_pos.x, pos_pj.y - this_pos.y, pos_pj.z - this_pos.z);
	//std::cout << distance_vec.x << " " << distance_vec.y << " " << distance_vec.z << std::endl;


	//Posicion global - real de las naves secundarias
	Matrix44 globalM = this->getGlobalMatrix();

	//Calculo el vector frontal de la nave
	Vector3 f = globalM.frontVector().normalize();

	//Guardo la posición inicial de la nave
	Vector3 pos_init = Vector3(globalM.m[12], globalM.m[13], globalM.m[14]);
	Vector3 pos_player = Vector3(g_scene.player->getGlobalMatrix().m[12], g_scene.player->getGlobalMatrix().m[13], g_scene.player->getGlobalMatrix().m[14]);
	//Calculo el vector que seguira la nave para llegar al punto (waypoint)
	Vector3 to_target = pos_player - pos_init;

	//Girar la nave hacia el lado que quiera
	float angle = acos(to_target.dot(f));

	//Calculo los ejes de la nave en esa dirección
	Vector3 axis = to_target.cross(f);

	//Calculo la matrix inversa de la nave
	Matrix44 inv = globalM;
	inv.inverse();

	//Calculo la rotación que tendrá que tomar la nave para llegar a su destino
	Vector3 rot_axis = inv.rotateVector(axis);

	if (distance > 150) {
		//Ajusto la velocidad a la que irán las naves
		float increaseSpeed = (float)30;

		//Se desplazan las naves con la velocidad adequada
		this->model.traslate(-f.x*dt*increaseSpeed, -f.y*dt*increaseSpeed, -f.z*dt*increaseSpeed);

		//Actualizo la posición nueva de la nave, despues de moverse
		globalM = this->getGlobalMatrix();

		//Calculo la distancia que hay entre la nave y el punto destino
		float distance = (waypoints[actualPoint] - Vector3(globalM.m[12], globalM.m[13], globalM.m[14])).length();

		//Calculo la distancia que ha recorrido la nave desde el punto inicial
		float distanceMoved = (pos_init - Vector3(globalM.m[12], globalM.m[13], globalM.m[14])).length();

	}

	//Actualizo la posición nueva de la nave, despues de moverse
	globalM = this->getGlobalMatrix();

	int correct = rot_axis.length();
	//std::cout << "waypoint actual is = "<< actualPoint << std::endl;

	//Controlo la vibración de la nave-objeto fijando la rotación a 0
	if (correct == 0) {
		this->model.rotateLocal(0, rot_axis);
	}
	else {
		//Sino roto normal
		this->model.rotateLocal(-0.010, rot_axis);
	}
}

void GameObjectMesh::updateMoving(float dt) {
	//Posicion global - real de las naves secundarias
	Matrix44 globalM = this->getGlobalMatrix();

	//Calculo el vector frontal de la nave
	Vector3 f = globalM.frontVector().normalize();

	//Guardo la posición inicial de la nave
	Vector3 pos_init = Vector3(globalM.m[12], globalM.m[13], globalM.m[14]);

	//Calculo el vector que seguira la nave para llegar al punto (waypoint)
	Vector3 to_target = waypoints[actualPoint] - pos_init;

	//Girar la nave hacia el lado que quiera
	float angle = acos(to_target.dot(f));

	//Calculo los ejes de la nave en esa dirección
	Vector3 axis = to_target.cross(f);

	//Calculo la matrix inversa de la nave
	Matrix44 inv = globalM;
	inv.inverse();

	//Calculo la rotación que tendrá que tomar la nave para llegar a su destino
	Vector3 rot_axis = inv.rotateVector(axis);

	//Ajusto la velocidad a la que irán las naves
	float increaseSpeed = (float)30;

	//Se desplazan las naves con la velocidad adequada
	this->model.traslate(-f.x*dt*increaseSpeed, -f.y*dt*increaseSpeed, -f.z*dt*increaseSpeed);

	//Actualizo la posición nueva de la nave, despues de moverse
	globalM = this->getGlobalMatrix();

	//Calculo la distancia que hay entre la nave y el punto destino
	float distance = (waypoints[actualPoint] - Vector3(globalM.m[12], globalM.m[13], globalM.m[14])).length();

	//Calculo la distancia que ha recorrido la nave desde el punto inicial
	float distanceMoved = (pos_init - Vector3(globalM.m[12], globalM.m[13], globalM.m[14])).length();


	int correct = rot_axis.length();
	//std::cout << "waypoint actual is = "<< actualPoint << std::endl;

	//Controlo la vibración de la nave-objeto fijando la rotación a 0
	if (correct == 0) {
		this->model.rotateLocal(0, rot_axis);
	}
	else {
		//Sino roto normal
		this->model.rotateLocal(-0.005, rot_axis);
	}

	//std::cout << "distanceMoved is = " << distanceMoved << std::endl;
	//std::cout << "distance is = " << distance << std::endl;

	//Controlo si ha llegado al destino
	if (distanceMoved >= (int)distance) {

		//Si es asi actualizo el nuevo punto destino
		actualPoint++;
		if (actualPoint >= waypoints.size()) {
			//Si no hay más puntos, reinicio los waypoints
			actualPoint = 0;
		}
	}



	//std::cout << "pos_init is = " << pos_init.x << std::endl;
	//std::cout << "pos_actual is = " << globalM.m[12] << std::endl;
}

void  GameObjectMesh::onCollideBullet(float power) {
	this->hp = this->hp - power;
	g_scene.scene_modificated = false;

	if (hp < 0) {
		g_scene.enemies--;
		Matrix44 model = this->parent->getGlobalMatrix();

		for (int i = 0; i < this->children.size(); i++) {
			g_scene.enemies--;
		}
		this->children.clear();

		this->parent->removeChild(this);
	}

}