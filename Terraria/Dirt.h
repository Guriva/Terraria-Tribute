#pragma once
#ifndef _DIRT_INCLUDE
#define _DIRT_INCLUDE

#include "Item.h"

class Dirt : public Item
{

public:
	Dirt(int id);
	Item* clone() const { return new Dirt(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
};

#endif // _DIRT_INCLUDE