#include "scene.h"

Scene::Scene() {
	this->root = new GameObject();
}
Scene::~Scene() {
	this->root->clear();
	delete this->root;
}
void Scene::render() {
	this->root->render();
	this->g_bulletManager->renderOptimized();
	this->g_bulletEnemyManager->renderBad();
	//this->g_bulletManager->renderBad();
	
	this->hud->render();
}
void Scene::update(float dt) {
	this->root->update(dt);
	this->g_bulletManager->update(dt);
	this->g_bulletEnemyManager->update(dt);
	//this->checkCollions();
}void Scene::addPlayer(GameObjectPlayer* player) {	this->root->addChild(player);	this->player = player;}//recursive function which fills a vector (passed by reference) of gameobjects//only if they have property 'type' set to "enemy"void Scene::getAllEnemies(std::vector<GameObject*> &enemies, GameObject* current) {	if (current->type == "enemy"){		enemies.push_back(current);	}	for (size_t i = 0; i < current->children.size(); i++) {		getAllEnemies(enemies, current->children[i]);	}}