#include "HUD.h"

HUD::HUD(int sw, int sh){
	cam = new Camera();
	cam->setOrthographic(0, sw, 0, sh, 0.1, 2);
	cam->updateViewMatrix();

	//create mesh and resize verts
	minimap_back_mesh = new Mesh();
	minimap_back_mesh->vertices.resize(4);
	minimap_back_mesh->uvs.resize(4);

	minimap_back_mesh->vertices[0] = Vector3(sw - 256, 0, 1); minimap_back_mesh->uvs[0] = Vector2(0, 0);
	minimap_back_mesh->vertices[1] = Vector3(sw, 0, 1); minimap_back_mesh->uvs[1] = Vector2(1, 0);
	minimap_back_mesh->vertices[2] = Vector3(sw, 256, 1); minimap_back_mesh->uvs[2] = Vector2(1, 1);
	minimap_back_mesh->vertices[3] = Vector3(sw - 256, 256, 1); minimap_back_mesh->uvs[3] = Vector2(0, 1);

	minimap_center = Vector3((sw - 256 + sw)/2, (256 + 0) / 2, 1);

	//create indices 
	minimap_back_mesh->indices.resize(6);
	minimap_back_mesh->indices[0] = 0; minimap_back_mesh->indices[1] = 1; minimap_back_mesh->indices[2] = 2;
	minimap_back_mesh->indices[3] = 0; minimap_back_mesh->indices[4] = 2; minimap_back_mesh->indices[5] = 3;

	//send to GPU
	minimap_back_mesh->genBuffers();

	tex_shader = new Shader();
	tex_shader->load("data/shaders/simple.vert", "data/shaders/enviroment_sky.frag");
	minimap_back_tex = new Texture();
	minimap_back_tex->load("data/assets/compass_256.tga");
	

	this->point_shader = new Shader();
	this->point_shader->load("data/shaders/aabb.vert", "data/shaders/aabb.frag");

	//create mesh and resize verts
	poiter_mesh = new Mesh();
	poiter_mesh->vertices.resize(4);
	poiter_mesh->uvs.resize(4);
	/*
	poiter_mesh->vertices[0] = Vector3(sw - 10, -10, 1); poiter_mesh->uvs[0] = Vector2(0, 0);
	poiter_mesh->vertices[1] = Vector3(sw, -10, 1); poiter_mesh->uvs[1] = Vector2(1, 0);
	poiter_mesh->vertices[2] = Vector3(sw, 10, 1); poiter_mesh->uvs[2] = Vector2(1, 1);
	poiter_mesh->vertices[3] = Vector3(sw - 10, 10, 1); poiter_mesh->uvs[3] = Vector2(0, 1);
	*/
	poiter_mesh->vertices[0] = Vector3(0, 0, 1); poiter_mesh->uvs[0] = Vector2(0, 0);
	poiter_mesh->vertices[1] = Vector3(sw, 0, 1); poiter_mesh->uvs[1] = Vector2(1, 0);
	poiter_mesh->vertices[2] = Vector3(sw, sh, 1); poiter_mesh->uvs[2] = Vector2(1, 1);
	poiter_mesh->vertices[3] = Vector3(0, sh, 1); poiter_mesh->uvs[3] = Vector2(0, 1);
	
	//create indices 
	poiter_mesh->indices.resize(6);
	poiter_mesh->indices[0] = 0; minimap_back_mesh->indices[1] = 1; minimap_back_mesh->indices[2] = 2;
	poiter_mesh->indices[3] = 0; minimap_back_mesh->indices[4] = 2; minimap_back_mesh->indices[5] = 3;

	//send to GPU
	poiter_mesh->genBuffers();

	pointer_tex = new Texture();
	pointer_tex->load("data/assets/empezar.tga");

}

void HUD::render() {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set uniforms and draw texture quad 
	tex_shader->enable();
	tex_shader->setTexture("u_texture_diffuse", minimap_back_tex);
	tex_shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	minimap_back_mesh->render(GL_TRIANGLES, tex_shader);
	//poiter_mesh->render(GL_TRIANGLES, tex_shader);
	tex_shader->disable();

	//re-enable depth test (por si las moscas)
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

}

void HUD::radarRender(GameObject* player) {
	Mesh* minimap_points_mesh = new Mesh();
	minimap_points_mesh->vertices.clear();

	for (int i = 0; i < this->all_enemies.size(); i++) {
		Matrix44 globalM = player->getGlobalMatrix();

		//Calculo el vector frontal de la nave
		Vector3 f = globalM.frontVector().normalize();
		f.y = 0;

		//Guardo la posición inicial de la nave
		Vector3 pos_player = Vector3(globalM.m[12], globalM.m[13], globalM.m[14]);
		Vector3 pos_enemy = Vector3(this->all_enemies[i]->getGlobalMatrix().m[12], this->all_enemies[i]->getGlobalMatrix().m[13], this->all_enemies[i]->getGlobalMatrix().m[14]);
			//Calculo el vector que seguira la nave para llegar al punto (waypoint)
		Vector3 PE = pos_enemy - pos_player;
		PE.y = 0;

		//Girar la nave hacia el lado que quiera
		float angle = acos(PE.dot(f));

		float enemy_distance = PE.length();

		Vector3 axis = PE.cross(f);
		if (axis.y < 0) {
			angle = -angle;
		}

		float max_distance = 1400;

		float minimap_radius = 100;
		float radius = (minimap_radius / max_distance) *enemy_distance;
		Vector3 C = minimap_center;

		Vector3 Enemy_position = Vector3(C.x + radius * sin(angle), C.y + radius*cos(angle), 1);

		minimap_points_mesh->vertices.push_back(Enemy_position);

	}

		Matrix44 mvp;
		mvp.setScale(1.0f, 1.0f, 1.0f);


		glPointSize(7.0);		point_shader->enable();
		point_shader->setMatrix44("u_mvp", mvp);
		minimap_points_mesh->render(GL_TRIANGLES, point_shader);
		point_shader->disable();

}