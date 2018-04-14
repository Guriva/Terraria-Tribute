#pragma once
#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE

#include <string>
#include "TileMap.h"
#include "Sprite.h"

class Item
{

public:
	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPosPlayer(glm::ivec2 posP);
	string getName();
	bool getCollides();
	void setName(string n);
	int getMaxStack();
	int getStack();
	void setStack(int n);
	glm::ivec2 getSizeItem();
	void setSizePlayer(int s);
	bool hasTileNear(glm::vec2 posTile);
	bool isClose(glm::ivec2 posPlayer, glm::ivec2 sizePlayer, glm::ivec2 posItem, glm::ivec2 sizeItem, int dist);
	bool collision(glm::ivec2 posPlayer, glm::ivec2 sizePlayer, glm::ivec2 posItem, glm::ivec2 sizeItem);
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap) = 0;
	virtual Item* clone() const = 0;
	virtual bool use(glm::ivec2 posMouse) = 0;
	virtual int getSound() { return 0; }
	virtual vector<Item*> itemsToAdd() { return itemsQueue; }
	Sprite* getSprite() { return sprite; }
	void update(bool collect);
	vector<pair<int, string>> needForCraft() { return toCraft; }

protected:
	string name;
	int id_item, stack, max_stack, sizeInv, sound;
	bool collides, collectable;
	glm::ivec2 tileMapDispl, posItem, posPlayer, sizeItem;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	vector<Item*> itemsQueue;
	vector<pair<int, string>> toCraft;
	ShaderProgram program;

};

#endif // _ITEM_INCLUDE