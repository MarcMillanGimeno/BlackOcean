#include "utils.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

#include "includes.h"

#include "game.h"
#include "camera.h"
#include "shader.h"

#include "extra/stb_easy_font.h"

//quick dirty global
GLuint grid_vao;
GLuint grid_vbo_vert;
GLuint grid_vbo_ind;
GLuint grid_num_points;
Shader* grid_shader;

long getTime()
{
	#ifdef WIN32
		return GetTickCount();
	#else
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return (int)(tv.tv_sec*1000 + (tv.tv_usec / 1000));
	#endif
}

void genGrid(float dist, float n_step) {

	float xs = -dist / 2; float xe = dist / 2;
	float ys = -dist / 2; float ye = dist / 2;
	float zs = -dist / 2; float ze = dist / 2;

	float x_size = dist;
	float y_size = dist;
	float z_size = dist;

	float x_step = x_size / n_step;
	float y_step = y_size / n_step;
	float z_step = z_size / n_step;

	int n = (int)n_step + 1;

	std::vector<Vector3> grid;
	grid.resize(n*n*n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				Vector3 p(xs + x_step * i, ys + y_step * j, zs + z_step *k);
				grid[i + n*j + n*n*k] = p;
			}
		}
	}
	std::vector<unsigned int> indices;
	for (size_t i = 0; i < grid.size(); i++) {
		indices.push_back((unsigned int)i);
	}

	//bind vao to store state
	glGenVertexArrays(1, &grid_vao);
	glBindVertexArray(grid_vao);

	glGenBuffers(1, &grid_vbo_vert); //generate one handler (id)
	glBindBuffer(GL_ARRAY_BUFFER, grid_vbo_vert); //bind the handler
	glBufferData(GL_ARRAY_BUFFER, grid.size() * 3 * sizeof(float), &grid[0], GL_STATIC_DRAW); //upload data
	glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
	glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &grid_vbo_ind); //create more new buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_vbo_ind); //bind them as element array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	grid_shader = new Shader();
	if (!grid_shader->load("data/shaders/simple.vert", "data/shaders/simple.frag"))
	{
		std::cout << "grid shader not found or error" << std::endl;
		exit(0);
	}

	grid_num_points = (GLuint)grid.size();

	glBindVertexArray(0); //unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffers

}

//Draw the grid
void drawGrid()
{
	Matrix44 m;
	grid_shader->enable();
	grid_shader->setMatrix44("u_model", m);
	grid_shader->setMatrix44("u_mvp", m);

	glBindVertexArray(grid_vao);

	glDrawElements(GL_LINES, grid_num_points, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	//disable the shader
	grid_shader->disable();
}

//this function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}

//Retrieve the current path of the application
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#ifdef WIN32
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
	#define GetCurrentDir getcwd
#endif

std::string getPath()
{
    std::string fullpath;
    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    chdir(path);
    fullpath = path;
#else
	 char cCurrentPath[1024];
	 if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		 return "";

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	fullpath = cCurrentPath;

#endif    
    return fullpath;
}


bool checkGLErrors()
{
	#ifdef _DEBUG
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error: " << errString << std::endl;
		return false;
	}
	#endif

	return true;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


Vector2 getDesktopSize( int display_index )
{
  SDL_DisplayMode current;
  // Get current display mode of all displays.
  int should_be_zero = SDL_GetCurrentDisplayMode(display_index, &current);
  return Vector2( (float)current.w, (float)current.h );
}


bool drawText(float x, float y, std::string text, Vector3 c, float scale )
{
	static char buffer[99999]; // ~500 chars
	int num_quads;

	num_quads = stb_easy_font_print(x, y, (char*)(text.c_str()), NULL, buffer, sizeof(buffer));

	Camera cam;
	cam.setOrthographic(0, Game::instance->window_width / scale, Game::instance->window_height / scale, 0, -1, 1);
	cam.set();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

/*#ifndef __APPLE__
	glColor3f(c.x, c.y, c.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 16, buffer);
	glDrawArrays(GL_QUADS, 0, num_quads * 4);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
*/

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}
