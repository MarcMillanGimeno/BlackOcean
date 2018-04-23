#include "menu.h"
#include "game.h"
#include "bass.h"

Menu::Menu(int sw, int sh) {
	cam = new Camera();
	cam->setOrthographic(0, sw, 0, sh, 0.1, 2);
	cam->updateViewMatrix();


	//create mesh and resize verts and uvs 
	mesh = new Mesh();
	mesh->vertices.resize(4);
	mesh->uvs.resize(4);

	//fill verts and uvs 
	/*
	mesh->vertices[0] = Vector3(-1, -1, 0); mesh->uvs[0] = Vector2(0, 0);
	mesh->vertices[1] = Vector3(1, -1, 0); mesh->uvs[1] = Vector2(1, 0);
	mesh->vertices[2] = Vector3(1, 1, 0); mesh->uvs[2] = Vector2(1, 1);
	mesh->vertices[3] = Vector3(-1, 1, 0); mesh->uvs[3] = Vector2(0, 1);
	*/
	mesh->vertices[0] = Vector3(0, 0, 1); mesh->uvs[0] = Vector2(0, 0);
	mesh->vertices[1] = Vector3(sw, 0, 1); mesh->uvs[1] = Vector2(1, 0);
	mesh->vertices[2] = Vector3(sw, sh, 1); mesh->uvs[2] = Vector2(1, 1);
	mesh->vertices[3] = Vector3(0, sh, 1); mesh->uvs[3] = Vector2(0, 1);


	//create indices 
	mesh->indices.resize(6);
	mesh->indices[0] = 0; mesh->indices[1] = 1; mesh->indices[2] = 2;
	mesh->indices[3] = 0; mesh->indices[4] = 2; mesh->indices[5] = 3;

	//send to GPU
	mesh->genBuffers();

	Texture *texture = new Texture();
	texture->load("data/assets/splash.tga");
	this->Splash_screen = texture;

	texture = new Texture();
	texture->load("data/assets/loading.tga");
	this->Loading_screen = texture;


	texture = new Texture();
	texture->load("data/assets/Congratulations-Message.tga");
	this->Win = texture;

	texture = new Texture();
	texture->load("data/assets/GameOver-Messaage.tga");
	this->Lost = texture;


	texture = new Texture();
	texture->load("data/assets/Alert.tga");
	this->mission = texture;


	Shader *shader_menu = new Shader();
	if (!shader_menu->load("data/shaders/simple.vert", "data/shaders/enviroment_sky.frag"))
	{
		std::cout << "shader not found or error" << std::endl;
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}
	this->shader = shader_menu;

	
	//Inicializamos Bass (id_del_device, muestras por segundo, ...)
	BASS_Init(1, 44100, 0, 0, NULL);

	//Cargamos un sample (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound
	HSAMPLE hSample = BASS_SampleLoad(false, "data/assets/bso.wav", 0, 0, 3, BASS_SAMPLE_LOOP);

	//Load the sample into a channel - this could be a class property
	HCHANNEL musicChannel = BASS_SampleGetChannel(hSample, false);

	//Play the channel
	BASS_ChannelPlay(musicChannel, true);
	
}

Menu::~Menu() {
	
}

void Menu::render() {

	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->entries[this->current_entry]);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();
}

void Menu::update(SDL_Window* window) {
	if (this->entries.size() != 1) {
		if (primero) {
			primero = false;
			t0 = clock();
		}
		else {
			t1 = clock();
			double time = (double(t1 - t0) / CLOCKS_PER_SEC);
			if (time > 0.1) {
				if (Game::instance->keystate[SDL_SCANCODE_S] || Game::instance->keystate[SDL_SCANCODE_DOWN]) {
					if (this->current_entry == this->entries.size() -1) {
						this->current_entry = 0;

					}

					else {
						this->current_entry++;
					}
					t0 = t1;
				}
				if (Game::instance->keystate[SDL_SCANCODE_W] || Game::instance->keystate[SDL_SCANCODE_UP]) {
					if (this->current_entry == 0) {
						this->current_entry = this->entries.size() - 1;
					}

					else {
						this->current_entry--;
					}
					t0 = t1;
				}

				if (Game::instance->keystate[SDL_SCANCODE_SPACE]) {
					this->clickControl(window);
					t0 = t1;

				}
			}
		}
	}
}

void Menu::clickControl(SDL_Window* window) {
	switch (current_entry) {
	case 0:
		this->act = false;
		this->initGame(window);
		break;
	case 1:
		this->ins = true;
		break;
	case 2:
		exit(0);
		break;
	default:
		break;
	}
}

void Menu::renderIns() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->Instruc);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();
}

void Menu::exitIns() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->Instruc);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();


	if (primero) {
		primero = false;
		t0 = clock();
	}
	else {
		t1 = clock();
		double time = (double(t1 - t0) / CLOCKS_PER_SEC);
		if (time > 0.1) {
			if (Game::instance->keystate[SDL_SCANCODE_SPACE]) {
				this->ins = false;
				t0 = t1;

			}
		}
	}
}

void Menu::initGame(SDL_Window* window) {
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->mission);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();


	//disable blending to draw text on top
	glDisable(GL_BLEND);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(window);

	t0 = clock();
	while (true) {
		t1 = clock();
		double time = (double(t1 - t0) / CLOCKS_PER_SEC);

		if (time > 3) {
			break;
		}
	}
}

void Menu::LoadingScreen(SDL_Window* window) {
	if (primero) {
		primero = false;

		t0 = clock();
	}

	else {
		t1 = clock();
		double time = (double(t1 - t0) / CLOCKS_PER_SEC);

		if (time > 0.2) {
			if (this->loading == 1) {
				loading = 2;
			}
			else {
				loading = 1;
			}
			t0 = t1;
		}
	}

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (this->loading == 1) {
		this->renderSplash();
	}
	else {
		this->renderSLoading();
	}

	//disable blending to draw text on top
	glDisable(GL_BLEND);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(window);
}

void Menu::renderSplash() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->Splash_screen);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();
}

void Menu::renderSLoading() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->Loading_screen);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();
}

void Menu::renderWin() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->Win);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();
}

void Menu::renderLost() {
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	this->shader->setTexture("u_texture_diffuse", this->Lost);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);


	//disable the shader
	this->shader->disable();
}



void Menu::finishGame(bool win, SDL_Window* window) {
	t0 = clock();

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (win) {
		this->renderWin();
	}

	else {
		this->renderLost();
	}


	//disable blending to draw text on top
	glDisable(GL_BLEND);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(window);


	while (true){
		t1 = clock();
		double time = (double(t1 - t0) / CLOCKS_PER_SEC);

		if (time > 4) {
			break;
		}
	}
	
	exit(0);
}