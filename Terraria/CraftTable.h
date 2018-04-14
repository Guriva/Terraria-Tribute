#pragma once
#ifndef _CRAFTTABLE_INCLUDE
#define _CRAFTTABLE_INCLUDE

#include "Item.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Inventory.h"

class CraftTable
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 posPlayer, Inventory *inv, TileMap *tileMap);
	void update(glm::vec2 posPlayer);
	void render(glm::mat4 projection);
	void setMousePos(glm::vec2 pos) { posMouse = pos; }
	int getSize();
private:
	glm::ivec2 tileMapDispl, pos, posMouse;
	TileMap *map;
	vector<Item*> items;
	Texture invBack, invBack2;
	TexturedQuad *layoutCT[2];
	ShaderProgram program;
	bool open, key, click;
	Text text;
	char textI[20];
	Inventory *inventory;
};

#endif // _CRAFTTABLE_INCLUDE