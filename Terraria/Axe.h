#pragma once
#ifndef _AXE_INCLUDE
#define _AXE_INCLUDE

#include "Item.h"

class Axe : public Item
{

public:
	Axe(int id);
	Item* clone() const { return new Axe(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
	vector<Item*> Axe::itemsToAdd();
	int getSound();
};

#endif // _AXE_INCLUDE