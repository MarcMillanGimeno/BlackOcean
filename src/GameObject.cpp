#include "GameObject.h"
#include <algorithm>
#include "managers.h"
#include "GameObjectEnviroment.h"
#include <typeinfo>


void GameObject::addChild(GameObject* obj) {
	//make this object the parent of the child
	obj->parent = this;
	//add to list
	this->children.push_back(obj);
}
void GameObject::removeChild(GameObject* obj) {
	//moves obj to end of vector
	std::vector<GameObject*>::iterator i = std::remove(this->children.begin(),
		this->children.end(), obj);
	//actually removes it
	this->children.erase(i, this->children.end());
}

Matrix44 GameObject::getGlobalMatrix() {
	if (this->parent)
		return this->parent->getGlobalMatrix() * this->model;
	else
		return this->model;
}
void GameObject::render() {
	//for (int i = 1; i < 2; i++) {
	for (int i = 0; i < this->children.size(); i++) {
		//std::cout <<"NAME: "<<this->children[i]->name << std::endl;
		//std::cout <<"SIZE: " << this->children[i]->mesh->vertices.size() << std::endl;
		this->children[i]->render();
	}
}

void GameObject::update(float elapsed_time) {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(elapsed_time);
	}
}

void GameObject::clear(){}

