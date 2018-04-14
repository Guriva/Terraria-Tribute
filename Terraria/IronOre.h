#pragma once
#ifndef _IRONORE_INCLUDE
#define _IRONORE_INCLUDE

#include "Item.h"

class IronOre : public Item
{

public:
	IronOre(int id);
	Item* clone() const { return new IronOre(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
};

#endif // _IRONORE_INCLUDE