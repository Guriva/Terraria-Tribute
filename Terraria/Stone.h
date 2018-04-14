#pragma once
#ifndef _STONE_INCLUDE
#define _STONE_INCLUDE

#include "Item.h"

class Stone : public Item
{

public:
	Stone(int id);
	Item* clone() const { return new Stone(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
};

#endif // _STONE_INCLUDE