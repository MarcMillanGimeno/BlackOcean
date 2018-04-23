#pragma once

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "ctime"
#include "camera.h"


class Menu {
	public:
		Menu(int sw, int sh);
		~Menu();

		Camera* cam;
		Mesh* mesh;
		Shader* shader;
		std::vector<Texture*> entries;
		Texture* Splash_screen;
		Texture* Loading_screen;
		Texture* Instruc;
		Texture* Win;
		Texture* Lost;
		Texture* mission;
		int current_entry = 0;
		double t0, t1;
		bool init = false;
		
		bool primero = true;
		bool act = true;
		bool ins = false;
		int loading = 1;

		void LoadingScreen(SDL_Window* window);
		void exitIns();
		void render();
		void renderIns();
		void renderSplash();
		void renderSLoading();
		void renderWin();
		void renderLost();
		void update(SDL_Window* window);
		void clickControl(SDL_Window* window);
		void initGame(SDL_Window* window);
		void finishGame(bool win, SDL_Window* window);
};