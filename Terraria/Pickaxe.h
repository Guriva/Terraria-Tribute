#pragma once
#ifndef _PICKAXE_INCLUDE
#define _PICKAXE_INCLUDE

#include "Item.h"

class Pickaxe : public Item
{

public:
	Pickaxe(int id);
	Item* clone() const { return new Pickaxe(*this); }
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap);
	bool use(glm::ivec2 posMouse);
	vector<Item*> Pickaxe::itemsToAdd();
	int getSound();
};

#endif // _PICKAXE_INCLUDE