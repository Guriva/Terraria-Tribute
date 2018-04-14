#pragma once
#ifndef _INVENTORY_INCLUDE
#define _INVENTORY_INCLUDE

#include "Item.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Sound.h"

#define MAX_SIZE_INVENTORY 50

class Inventory
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 posPlayer, Sound s);
	void update(glm::vec2 posPlayer);
	void render(glm::mat4 projection);
	int getSize();
	void decrementStackActive();
	bool collectable(string name);
	void addItem(Item *it);
	vector<pair<int,int>> containsItems(int num, string n);
	Item* getActiveItem();
	void deleteItem(int i);
	void decrementStack(int s, int pos);
private:
	glm::ivec2 tileMapDispl, pos;
	TileMap *map;
	vector<Item*> items;
	Texture invBack,invBackActive;
	TexturedQuad *layoutInv[2];
	ShaderProgram program;
	int active;
	bool open, key;
	Text text;
	Sound sound;
	char textI[20];
};

#endif // _INVENTORY_INCLUDE