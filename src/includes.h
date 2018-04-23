/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)
	based on framework by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This file has all the includes so the app works in different systems.
	It is a little bit low level so do not worry about the code.
*/

#ifndef INCLUDES_H
#define INCLUDES_H

//under windows we need this file to make opengl work
#ifdef WIN32 
	#include <windows.h>
#endif

#ifndef APIENTRY
    #define APIENTRY
#endif

#ifdef WIN32
	#define USE_GLEW
	#define GLEW_STATIC
	#include <GL/glew.h>
	#pragma comment(lib, "glew32s.lib")
#endif


//SDL
//#pragma comment(lib, "SDL2.lib")
//#pragma comment(lib, "SDL2main.lib")
#include <SDL2/SDL.h>



//GLUT
#ifdef WIN32
    #include "GL/GLU.h"
    #include <SDL2/SDL_opengl.h>
#endif

#ifdef __APPLE__
    #include "OpenGL/gl3.h"
    #include "OpenGL/glext.h"   
#endif

#include <iostream>


//used to access opengl extensions
#define REGISTER_GLEXT(RET, FUNCNAME, ...) typedef RET ( * FUNCNAME ## _func)(__VA_ARGS__); FUNCNAME ## _func FUNCNAME = NULL; 
#define IMPORT_GLEXT(FUNCNAME) FUNCNAME = (FUNCNAME ## _func) SDL_GL_GetProcAddress(#FUNCNAME); if (FUNCNAME == NULL) { std::cout << "ERROR: This Graphics card doesnt support " << #FUNCNAME << std::endl; }


//SHADER BINDINGS
#define VERTEX_ATTRIBUTE_LOCATION 0
#define NORMAL_ATTRIBUTE_LOCATION 1
#define UV_ATTRIBUTE_LOCATION 2
#define COLOR_ATTRIBUTE_LOCATION 3







#endif
