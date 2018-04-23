#pragma once

#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "GameObject.h"

class HUD {
	public:
		HUD(int sw, int sh);
		~HUD();

		Camera* cam;
		Shader* tex_shader;
		Shader* point_shader;

		//minimap quad
		Mesh* minimap_back_mesh;
		Mesh* poiter_mesh;
		Texture* pointer_tex;
		Texture* minimap_back_tex;

		std::vector<GameObject*> all_enemies;
		Vector3 minimap_center;

		void radarRender(GameObject* player);
		void render();
		void update(float);

};