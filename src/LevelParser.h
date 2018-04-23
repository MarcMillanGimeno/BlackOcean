#pragma once

#include <iostream>
#include "GameObject.h"
#include "extra\rapidxml.hpp"
#include "extra\rapidxml_utils.hpp"
#include "extra\rapidxml_iterators.hpp"

using namespace rapidxml;

namespace LevelParser {
	void loadObject(xml_node<> *mesh_node, GameObject* parent, SDL_Window* window);
	void loadLevel(char* level_url, int window_width, int window_height, SDL_Window* window);
}