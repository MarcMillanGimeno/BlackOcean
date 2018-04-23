#include "LevelParser.h"
#include "managers.h"
#include "GameObjectEnviroment.h"
#include "GameObjectMesh.h"
#include "GameObjectPlayer.h"
#include "camera.h"
#include "windows.h"

namespace LevelParser {
	void loadObject(xml_node<> *mesh_node, GameObject* parent, SDL_Window* window) {
		g_scene.enemies++;
		g_scene.menu->LoadingScreen(window);
		GameObjectMesh* new_mesh = new GameObjectMesh();
		
		//Load properties from mesh_node
		new_mesh->name = mesh_node->first_node("name")->value();
		new_mesh->mesh = g_resourceManager.getMesh(mesh_node->first_node("mesh")->value());
		new_mesh->type_npc = mesh_node->first_node("npc_type")->value();
		new_mesh->type_npc_code = (int)atof(mesh_node->first_node("type_npc_code")->value());
		new_mesh->type = "enemy";
		new_mesh->mesh->setCollisionModel();
		new_mesh->mesh->setAABB();
		new_mesh->mesh->createAABBGeometry();
		new_mesh->shader = g_resourceManager.getShader(mesh_node->first_node("shader")->value());
		new_mesh->texture = g_resourceManager.getTexture(mesh_node->first_node("texture")->value());
		new_mesh->texture_spec = g_resourceManager.getTexture(mesh_node->first_node("texture_spec")->value());
		//Load the transformations
		xml_node<> *xml_node_tranform = mesh_node->first_node("transform");
		for (xml_node<> *child = xml_node_tranform->first_node(); child;
		child = child->next_sibling()) {
			g_scene.menu->LoadingScreen(window);
			if (strcmp(child->name(), "rotate_local") == 0) {
				new_mesh->model.rotateLocal(
					(float)atof(child->first_node("angle")->value()),
					Vector3((float)atof(child->first_node("x")->value()),
						(float)atof(child->first_node("y")->value()),
						(float)atof(child->first_node("z")->value()))
					);
			}
			if (strcmp(child->name(), "translate") == 0) {
				new_mesh->model.traslate(
						(float)atof(child->first_node("x")->value()),
						(float)atof(child->first_node("y")->value()),
						(float)atof(child->first_node("z")->value())
					);
			}
			if (strcmp(child->name(), "scale") == 0) {
				new_mesh->model.scale(
					(float)atof(child->first_node("x")->value()),
					(float)atof(child->first_node("y")->value()),
					(float)atof(child->first_node("z")->value())
					);
			}
		}
		//Parent - Child 
		parent->addChild(new_mesh);

		for (xml_node<> *child = mesh_node->first_node("object"); child;
			child = child->next_sibling("object")) {
			loadObject(child, new_mesh, window);
		}
	}

	void loadLevel(char* level_url, int window_width, int window_height, SDL_Window* window) {
		g_scene.menu->LoadingScreen(window);
		g_scene.menu->LoadingScreen(window);

		rapidxml::file<> xmlFile(level_url);
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		xml_node<> *scene = doc.first_node();

		xml_node<> *camera = scene->first_node("camera");
		xml_node<> *lookat = camera->first_node("lookat");
		xml_node<> *vec_aux1 = lookat->first_node("vector3_1");
		Vector3 vec1 = Vector3((float)atof(vec_aux1->first_node("x")->value()), (float)atof(vec_aux1->first_node("y")->value()), (float)atof(vec_aux1->first_node("z")->value()));
		xml_node<> *vec_aux2 = lookat->first_node("vector3_2");
		Vector3 vec2 = Vector3((float)atof(vec_aux2->first_node("x")->value()), (float)atof(vec_aux2->first_node("y")->value()), (float)atof(vec_aux2->first_node("z")->value()));
		xml_node<> *vec_aux3 = lookat->first_node("vector3_3");
		Vector3 vec3 = Vector3((float)atof(vec_aux3->first_node("x")->value()), (float)atof(vec_aux3->first_node("y")->value()), (float)atof(vec_aux3->first_node("z")->value()));
		g_scene.menu->LoadingScreen(window);


		//Create our camera
		Camera* camera_camera = new Camera(); //our global camera
		camera_camera->lookAt(vec1, vec2, vec3); //position the camera and point to 0,0,0
		g_scene.camera = camera_camera;

		g_scene.camera->setPerspective(70.0f, (float)window_width / (float)window_height, 0.1f, 10000.0f); //set the projection, we want to be perspective

		g_scene.menu->LoadingScreen(window);


		//Load resources 
		xml_node<> *resources = scene->first_node("resources");
		xml_node<> *resource_meshes = resources->first_node("meshes");
		for (xml_node<> *child = resource_meshes->first_node(); child;
		child = child->next_sibling())
		{	
			xml_node<> *name = child->first_node("name");
			xml_node<> *url = child->first_node("url");
			g_resourceManager.loadMesh(name->value(), url->value());
			g_scene.menu->LoadingScreen(window);
		}
	
		xml_node<> *resource_textures = resources->first_node("textures");

		for (xml_node<> *child = resource_textures->first_node(); child;
		child = child->next_sibling())
		{
			xml_node<> *name = child->first_node("name");
			xml_node<> *url = child->first_node("url");
			g_resourceManager.loadTexture(name->value(), url->value());
			g_scene.menu->LoadingScreen(window);
		}

		xml_node<> *resource_shaders = resources->first_node("shaders");

		for (xml_node<> *child = resource_shaders->first_node(); child;
		child = child->next_sibling())
		{
			xml_node<> *name = child->first_node("name");
			xml_node<> *vert = child->first_node("vert");
			xml_node<> *frag = child->first_node("frag");
			g_resourceManager.loadShader(name->value(), vert->value(), frag->value());
			g_scene.menu->LoadingScreen(window);
		}

		g_scene.menu->LoadingScreen(window);

		//Load the scene
		xml_node<> *root = scene->first_node("root");
		xml_node<> *xml_env = root->first_node("environment");
		xml_node<> *xml_player = root->first_node("player");

		GameObjectEnviroment* environment = new GameObjectEnviroment();

		environment->name = xml_env->first_node("name")->value();
		environment->mesh = g_resourceManager.getMesh(xml_env->first_node("mesh")->value());
		environment->shader = g_resourceManager.getShader(xml_env->first_node("shader")->value());
		environment->texture = g_resourceManager.getTexture(xml_env->first_node("texture")->value());
		environment->is_collider = false;
		g_scene.root->addChild(environment);
		g_scene.menu->LoadingScreen(window);
		
		GameObjectPlayer* player = new GameObjectPlayer();

		player->name = "player";
		player->player = true;
		player->mesh = g_resourceManager.getMesh(xml_player->first_node("mesh")->value());
		player->mesh->setCollisionModel();
		player->mesh->setLine(player->model.frontVector().normalize());
		player->shader = g_resourceManager.getShader(xml_player->first_node("shader")->value());
		player->texture = g_resourceManager.getTexture(xml_player->first_node("texture")->value());
		player->texture_spec = g_resourceManager.getTexture(xml_player->first_node("texture_spec")->value());
		player->hp = 35000;
		g_scene.menu->LoadingScreen(window);
		for (xml_node<> *child = root->first_node("object"); child;
		child = child->next_sibling("object")) {
			loadObject(child, g_scene.root, window);
		}

		//Load bullets
		xml_node<> *bullets = scene->first_node("bullets");

		char* name;
		char* shader;
		char* mesh;
		int limit = 0;
		g_scene.menu->LoadingScreen(window);
		limit = limit = (int)atof(bullets->first_node("limit")->value());

		for (xml_node<> *child = bullets->first_node("bullet"); child;
		child = child->next_sibling("bullet"))
		{
			name = child->first_node("name")->value();
			shader = child->first_node("shader")->value();
			mesh = child->first_node("mesh")->value();
		}
		Mesh* bulletMesh = g_resourceManager.getMesh(mesh);
		bulletMesh->setCollisionModel();
		g_scene.g_bulletManager = new BulletManager(bulletMesh, limit, Vector3(0.0f, 1.0f, 0.0f));
		g_scene.g_bulletManager->shader = g_resourceManager.getShader(shader);
		g_scene.g_bulletEnemyManager = new BulletManager(bulletMesh, limit, Vector3(1.0f, 0.0f, 0.0f));
		g_scene.g_bulletEnemyManager->bullet_scale= Vector3(5.0f, 5.0f, 5.0f);
		g_scene.g_bulletEnemyManager->shader = g_resourceManager.getShader(shader);
		g_scene.menu->LoadingScreen(window);
		xml_node<> *collision = scene->first_node("Collision");
		xml_node<> *aabb = collision->first_node("AABB");
		xml_node<> *shader_aabb = aabb->first_node("shader");
		g_scene.aabb_shader = g_resourceManager.getShader(shader_aabb->value());
		player->shader_line = g_resourceManager.getShader(shader_aabb->value());
		g_scene.addPlayer(player);

		xml_node<> *menu = scene->first_node("menu");
		xml_node<> *textures_menu = menu->first_node("textures");
		g_scene.menu->entries.clear();
		for (xml_node<> *child = textures_menu->first_node(); child;
		child = child->next_sibling())
		{
			Texture *texture = new Texture();
			texture->load(child->value());
			g_scene.menu->entries.push_back(texture);
		}
		Texture *texture_ins = new Texture();
		texture_ins->load("data/assets/Instrucciones.tga");
		g_scene.menu->Instruc = texture_ins;
		xml_node<> *menu_shaders = menu->first_node("shaders");
		xml_node<> *menu_vert = menu_shaders->first_node("vert");
		xml_node<> *menu_frag = menu_shaders->first_node("frag");
		g_scene.menu->LoadingScreen(window);
	
		for (int i = 0; i < g_scene.root->children.size(); i++) {
			if (g_scene.root->children[i]->children.size() > 0) g_scene.root->children[i]->hp = 800;
		}
	}
}