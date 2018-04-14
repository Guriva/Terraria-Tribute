#pragma once
#ifndef _SWORD_INCLUDE
#define _SWORD_INCLUDE

#include "Item.h"

class Sword : public Item
{

public:
	Sword(int id);
	Item* clone() const { return new Sword(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
	int getSound();
};

#endif // _Sword_INCLUDE