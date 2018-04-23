#include "colision.h"

Colision::Colision() {
	
	for (int i = 0; i < 8; i++) {
		this->vertices.push_back(Vector3(0.0, 0.0, 0.0));
	}
	
	/*
	this->max_x = 0;
	this->min_x = 0;
	this->max_y = 0;
	this->min_y = 0;
	this->max_z = 0;
	this->min_z = 0;
	*/
}

void Colision::checkVolum(Vector3 vertex) {
	
	max_x = this->vertices[1].x;
	min_x = this->vertices[0].x;
	max_y = this->vertices[0].y;
	min_y = this->vertices[2].y;
	max_z = this->vertices[4].z;
	min_z = this->vertices[0].z;
	
	if (vertex.x > max_x) max_x = vertex.x;
	if (vertex.x < min_x) min_x = vertex.x; 
	if (vertex.y > max_y) max_y = vertex.y;
	if (vertex.y < min_y) min_y = vertex.y;
	if (vertex.z > max_z) max_z = vertex.z;
	if (vertex.z < min_z) min_z = vertex.z;
	
	this->vertices[0] = Vector3(min_x, max_y, min_z);
	this->vertices[1] = Vector3(max_x, max_y, min_z);
	this->vertices[2] = Vector3(min_x, min_y, min_z);
	this->vertices[3] = Vector3(max_x, min_y, min_z);
	this->vertices[4] = Vector3(min_x, max_y, max_z);
	this->vertices[5] = Vector3(max_x, max_y, max_z);
	this->vertices[6] = Vector3(min_x, min_y, max_z);
	this->vertices[7] = Vector3(max_x, min_y, max_z);
	
}

void Colision::update(Matrix44 model) {
	for (int i = 0; i < 8; i++) {
		vertices[i] =  model * vertices[i];
	}
	max_x = this->vertices[1].x;
	min_x = this->vertices[0].x;
	max_y = this->vertices[0].y;
	min_y = this->vertices[2].y;
	max_z = this->vertices[4].z;
	min_z = this->vertices[0].z;
}