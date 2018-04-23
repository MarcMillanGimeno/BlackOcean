#include "GameObjectPlayer.h"
#include "managers.h"
#include "utils.h"
#include "game.h"
#include "bass.h"


void GameObjectPlayer::render() { 

	Camera* camera = g_scene.camera;
	Vector3 l_pos = g_scene.lights[0]->light_pos;
	//Vector3 l_pos(100.0f, 100.0f, 100.0f);
	//build a rotation matrix - angle is incremented in update()
	//create our mvp
	Matrix44 mvp = this->getGlobalMatrix() * camera->viewprojection_matrix;

	//create normal matrix
	Matrix44 inverse_m = model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();


	//enable shader and set uniforms
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
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
	this->mesh->aabb.center = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	this->shader_line->enable();
	//mvp = camera->viewprojection_matrix;
	this->shader_line->setMatrix44("u_mvp", mvp);
	this->mesh->renderLine(this->shader_line);
	this->shader_line->disable();
}

void GameObjectPlayer::update(float dt) {
	if (g_scene.debugg_camera == false) {
		this->model.rotateLocal(Game::instance->mouse_delta.x * g_scene.sensibilidad, Vector3(0, -1, 0));
		//this->model.rotateLocal(Game::instance->mouse_delta.y * 0.005f, g_scene.camera->getLocalVector(Vector3(1, 0, 0)));
		this->model.rotateLocal(Game::instance->mouse_delta.y * g_scene.sensibilidad, Vector3(-1, 0, 0));
		Vector3 f = this->model.frontVector().normalize();
		Vector3 p = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
		Vector3 u = Vector3(this->model.m[4], this->model.m[5], this->model.m[6]).normalize();
		if (Game::instance->keystate[SDL_SCANCODE_W] || Game::instance->keystate[SDL_SCANCODE_UP]) {
			this->model.traslate(-f.x*dt * 120, -f.y*dt * 120, -f.z*dt * 120);
		}
		if (Game::instance->keystate[SDL_SCANCODE_S] || Game::instance->keystate[SDL_SCANCODE_DOWN]) {
			this->model.traslate(f.x*dt * 120, f.y*dt * 120, f.z*dt * 120);
		}
		if (Game::instance->keystate[SDL_SCANCODE_A] || Game::instance->keystate[SDL_SCANCODE_LEFT]) {
			this->model.rotateLocal(dt* 2.0f, Vector3(0, 0, -1));
		}
		if (Game::instance->keystate[SDL_SCANCODE_D] || Game::instance->keystate[SDL_SCANCODE_RIGHT]) {
			this->model.rotateLocal(-dt* 2.0f, Vector3(0, 0, -1));
		}

		if (primero) {
			primero = false; 
			t0 = clock();
		}

		if (Game::instance->keystate[SDL_SCANCODE_SPACE]) {
			//if (aux == 0){
			t1 = clock();
			double time = (double(t1 - t0)/CLOCKS_PER_SEC);
			if (time > 0.2) {
				g_scene.g_bulletManager->createBullet(p, Vector3(-f.x * 300.0, -f.y * 300.0, -f.z * 300.0), 150, 100, g_scene.player, Vector3(1.0f, 0.0f, 0.0f));
				t0 = t1;
				
				//Inicializamos Bass (id_del_device, muestras por segundo, ...)
				BASS_Init(1, 44100, 0, 0, NULL);

				//Cargamos un sample (memoria, filename, offset, length, max, flags)
				//use BASS_SAMPLE_LOOP in the last param to have a looped sound
				HSAMPLE hSample = BASS_SampleLoad(false, "data/assets/fire.wav", 0, 0, 3, 0);

				//Load the sample into a channel - this could be a class property
				HCHANNEL musicChannel = BASS_SampleGetChannel(hSample, false);

				//Play the channel
				BASS_ChannelPlay(musicChannel, true);
			}

		}



		//g_scene.camera->eye = Vector3(p.x + f.x * 30, p.y + f.y * 30, p.z + f.z * 30);
		g_scene.camera->center = p;
		g_scene.camera->up = u.normalize();
		Vector3 ideal_pos = Vector3(p.x + f.x * 30 + u.x * 10, p.y + f.y * 30 + u.y * 10, p.z + f.z * 30 + u.z * 10);
		ideal_pos = g_scene.camera->eye.lerp(ideal_pos, dt*10);
		Vector3 ideal_target = Vector3(p.x, p.y, p.z)-f*100;
		g_scene.camera->lookAt(ideal_pos, ideal_target, g_scene.camera->up);

		//g_scene.camera->move(g_scene.camera->getLocalVector(Vector3(1.0f, 0.0f, 0.0f))*20);
		//g_scene.camera->move(Vector3(0.0f, 1.0f, 0.0f) * -10);
		//this->model.traslate(-f.x*dt * 40, -f.y*dt * 40, -f.z*dt * 40);
	}
}


