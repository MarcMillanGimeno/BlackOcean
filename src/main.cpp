/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)
	based on framework by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com

	MAIN:
	 + This file creates the window and the game instance. 
	 + It also contains the mainloop
	 + This is the lowest level, here we access the system to create the opengl Context
	 + It takes all the events from SDL and redirect them to the game
*/

#include "includes.h"
#include "managers.h"
#include "framework.h"
#include "mesh.h"
#include "camera.h"
#include "utils.h"
#include "game.h"
#include "scene.h"
#include "menu.h"

#include <iostream> //to output

long last_time = 0; //this is used to calcule the elapsed time between frames
ResourceManager g_resourceManager;
Scene g_scene;

Game* game = NULL;

// *********************************
//create a window using SDL
SDL_Window* createWindow(const char* caption, int width, int height, bool fullscreen = false)
{
    int multisample = 8;
    bool retina = true; //change this to use a retina display

	//set attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); //or 24
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//antialiasing (disable this lines if it goes too slow)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisample ); //increase to have smoother polygons

    //force GL version higher than 2 (for MacOS)
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );



	//create the window
	SDL_Window *window = SDL_CreateWindow(
                                          caption, 100, 100, width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|
                                          (retina ? SDL_WINDOW_ALLOW_HIGHDPI:0) |
                                          (fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0) );
	if(!window)
	{
		fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
		exit(-1);
	}
  
	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    
    char *version = (char*)glGetString(GL_VERSION);
    std::cout <<*version<<std::endl ;

	//in case of exit, call SDL_Quit()
	atexit(SDL_Quit);

	//get events from the queue of unprocessed events
	SDL_PumpEvents(); //without this line asserts could fail on windows

	//launch glew to extract the opengl extensions functions from the DLL
	#ifdef USE_GLEW
		glewInit();
	#endif

	return window;
}


//The application main loop
void mainLoop()
{
	SDL_Event sdlEvent;
	int x,y;

	SDL_GetMouseState(&x,&y);
	game->mouse_position.set((float)x, (float)y);

	while (1)
	{
		//read keyboard state and stored in keystate
		game->keystate = SDL_GetKeyboardState(NULL);

		//render frame
		game->render();

		//update events
		while(SDL_PollEvent(&sdlEvent))
		{
			switch(sdlEvent.type)
				{
					case SDL_QUIT: return; break; //EVENT for when the user clicks the [x] in the corner
					case SDL_MOUSEBUTTONDOWN: //EXAMPLE OF sync mouse input
						game->onMouseButton( sdlEvent.button );
						break;
					case SDL_MOUSEBUTTONUP:
						//...
						break;
					case SDL_KEYDOWN: //EXAMPLE OF sync keyboard input
						game->onKeyPressed( sdlEvent.key );
						break;
					case SDL_WINDOWEVENT:
						switch (sdlEvent.window.event) {
							case SDL_WINDOWEVENT_RESIZED: //resize opengl context
								game->setWindowSize( sdlEvent.window.data1, sdlEvent.window.data2 );
								break;
						}
				}
		}

		//get mouse position and delta (do after pump events)
		game->mouse_state = SDL_GetMouseState(&x,&y);
		game->mouse_delta.set( game->mouse_position.x - x, game->mouse_position.y - y );
		game->mouse_position.set((float)x, (float)y);
        
        
		//update logic
        long now = SDL_GetTicks();
		double elapsed_time = (now - last_time) * 0.001; //0.001 converts from milliseconds to seconds
		last_time = now;
        game->time = now * 0.001f;
		game->update(elapsed_time); 

		//check errors in opengl only when working in debug
		#ifdef _DEBUG
			checkGLErrors();
		#endif
	}

	return;
}

int main(int argc, char **argv)
{

	//prepare SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	bool fullscreen = false; //change this to go fullscreen
	Vector2 size(800,600);

	if(fullscreen)
		size = getDesktopSize(0);

	//create the game window (WINDOW_WIDTH and WINDOW_HEIGHT are two macros defined in includes.h)
	SDL_Window* window = createWindow("Game Programming", (int)size.x, (int)size.y, fullscreen );
	if (!window)
		return 0;

	//launch the game (game is a global variable)
	game = new Game(window);
	game->init();

	//main loop, application gets inside here till user closes it
	mainLoop();

	//save state and free memory
	//...

	return 0;
}
