#pragma once
#ifndef _WOOD_INCLUDE
#define _WOOD_INCLUDE

#include "Item.h"

class Wood : public Item
{

public:
	Wood(int id);
	Item* clone() const { return new Wood(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
};

#endif // _WOOD_INCLUDE