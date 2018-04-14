#pragma once
#ifndef _GOLDORE_INCLUDE
#define _GOLDORE_INCLUDE

#include "Item.h"

class GoldOre : public Item
{

public:
	GoldOre(int id);
	Item* clone() const { return new GoldOre(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
};

#endif // _GOLDORE_INCLUDE