#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "managers.h"
#include <cmath>
#include "GameObjectMesh.h"
#include "GameObjectEnviroment.h"
#include "GameObjectLight.h"
#include "LevelParser.h"

//some globals
RenderToTexture* rt = NULL;

Game* Game::instance = NULL;
float angle = 0;
int time_bottom = 1000; 

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	//std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	mouse_locked = false;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
	//OpenGL flags
	glEnable(GL_CULL_FACE); //render both sides of every triangle
	glEnable(GL_DEPTH_TEST); //check the occlusions using the Z buffer


	g_scene.menu = new Menu(window_width, window_height);
	
	g_scene.menu->LoadingScreen(window);

	g_scene.hud = new HUD(window_width, window_height);

	LevelParser::loadLevel("data/assets/assets.xml", window_width, window_height, window);

	
	//Lights
	g_scene.lights.push_back(new GameObjectLight(100.0f, 100.0f, 100.0f));

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update view and projection matrices, and push gl matrices for fixed pipeline rendering
	g_scene.camera->set();

	if (g_scene.state == 0) {
		g_scene.menu->render();
	}
	else if (g_scene.state == 1){
		//scene.render();
		g_scene.render();
	}

	else if (g_scene.state == 2) {
		g_scene.menu->renderIns();
	}


	//disable blending to draw text on top
    glDisable( GL_BLEND );

	//*** Drawing code ends here ***

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	double speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

	ShowCursor(false);
	if (g_scene.state == 0){
		g_scene.menu->update(window);
		
		if (!g_scene.menu->act) {
			g_scene.state = 1;
		}
		else if (g_scene.menu->ins) {
			g_scene.state = 2;
		}
	}
	

	else if (g_scene.state == 1) {
		//std::cout << g_scene.enemies << std::endl;
		if (g_scene.player->hp < 0) {
			g_scene.menu->finishGame(false, window);
		}
		else {
			if (g_scene.enemies == 0) {
				g_scene.menu->finishGame(true, window);
			}
		}

		if (time_bottom < 500) {
			time_bottom++;
		}

		else {
			if (keystate[SDL_SCANCODE_P]) {
				if (g_scene.debugg_camera == true) {
					//std::cout << "Mode debugg disable" << std::endl;
					g_scene.camera = g_scene.camera_aux;
					g_scene.debugg_camera = false;
				}
				else {
					//std::cout << "Mode debugg enable" << std::endl;
					g_scene.camera_aux = g_scene.camera;
					g_scene.debugg_camera = true;
				}
				time_bottom = 0;
			}
		}


		if (g_scene.debugg_camera == true) {
			//mouse input to rotate the cam

			int center_x = (int)floor(window_width*0.5f);
			int center_y = (int)floor(window_height*0.5f);
			//center_x = center_y = 50;
			SDL_WarpMouseInWindow(this->window, center_x, center_y);

			this->mouse_position.x = (float)center_x;
			this->mouse_position.y = (float)center_y;

			if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
			{
				g_scene.camera->rotate(mouse_delta.x * 0.005f, Vector3(0, -1, 0));
				g_scene.camera->rotate(mouse_delta.y * 0.005f, g_scene.camera->getLocalVector(Vector3(-1, 0, 0)));

			}

			//async input to move the camera around
			if (keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
			if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
				g_scene.camera->move(Vector3(0, 0, 1) * (float)speed);
			}
			if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
				g_scene.camera->move(Vector3(0, 0, -1) * (float)speed);
			}
			if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
				g_scene.camera->move(Vector3(1, 0, 0) * (float)speed);
			}
			if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
				g_scene.camera->move(Vector3(-1, 0, 0) * (float)speed);
			}

		}
		else {
			int center_x = (int)floor(window_width*0.5f);
			int center_y = (int)floor(window_height*0.5f);
			//center_x = center_y = 50;
			SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
																	 //SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen
			this->mouse_position.x = (float)center_x;
			this->mouse_position.y = (float)center_y;

			g_scene.update((float)seconds_elapsed);
		}

		angle += (float)seconds_elapsed * 10;
	}
	else if(g_scene.state == 2) {
		g_scene.menu->exitIns();
		if (!g_scene.menu->ins) {
			g_scene.state = 0;
		}
	}
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
	//std::cout << "window resized: " << width << "," << height << std::endl;
    
	/*
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	*/

	glViewport( 0,0, width, height );
	g_scene.camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

