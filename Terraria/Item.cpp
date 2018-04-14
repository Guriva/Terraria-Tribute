#include <cmath>
#include <iostream>
#include "Item.h"

#define FALL_STEP 4

void Item::render()
{
	sprite->render();
}

string Item::getName() {
	return name;
}

void Item::setName(string n) {
	name = n;
}

void Item::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

bool Item::isClose(glm::ivec2 posPlayer, glm::ivec2 posEndPlayer, glm::ivec2 posItem, glm::ivec2 posEndItem, int dist) {
	glm::ivec2 midPointPl = { (posPlayer.x + posEndPlayer.x) / 2, (posPlayer.y + posEndPlayer.y) / 2 };
	glm::ivec2 midPointIt = { (posItem.x + posEndItem.x) / 2, (posItem.y + posEndItem.y) / 2 };
	return (abs(midPointPl.x - midPointIt.x) < dist && abs(midPointPl.y - midPointIt.y) < dist);
}

void Item::setPosition(const glm::vec2 &pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

void Item::setPosPlayer(glm::ivec2 posP) {
	posPlayer = posP;
}

bool Item::getCollides() {
	return collides;
}

bool Item::collision(glm::ivec2 posPlayer, glm::ivec2 sizePlayer, glm::ivec2 posItem, glm::ivec2 sizeItem) {
	if (posPlayer.x < posItem.x + sizeItem.x &&
		posPlayer.x + sizePlayer.x > posItem.x &&
		posPlayer.y < posItem.y + sizeItem.y &&
		sizePlayer.y + posPlayer.y > posItem.y) return true;
	return false;
}

void Item::update(bool collect) {
	collectable = collect;
	if (isClose(posPlayer + glm::ivec2(9, 0), posPlayer + glm::ivec2(22, 32), posItem, posItem + glm::ivec2(16, 16), 32) && collectable) {
		glm::ivec2 midPointPl = { (posPlayer.x + 9 + posPlayer.x + 22) / 2, (posPlayer.y + posPlayer.y + 32) / 2 };
		glm::ivec2 midPointIt = { (posItem.x + posItem.x + 16) / 2, (posItem.y + posItem.y + 16) / 2 };
		if (midPointPl.x > midPointIt.x) posItem.x += 1;
		else if (midPointPl.x < midPointIt.x) posItem.x -= 1;
		if (midPointPl.y > midPointIt.y) posItem.y += 1;
		else if (midPointPl.y < midPointIt.y) posItem.y -= 1;
		if (collision(midPointPl, glm::ivec2(0, 0), posItem, glm::ivec2(16, 16))) {
			collides = true;
		}
	}
	else {
		posItem.y += FALL_STEP;
		map->collisionMoveDown(posItem, glm::ivec2(16, 16), &posItem.y);
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

void Item::setSizePlayer(int s) {
	sizeInv = s;
}

int Item::getMaxStack() {
	return max_stack;
}

int Item::getStack() {
	return stack;
}

void Item::setStack(int n) {
	stack = n;
}

glm::ivec2 Item::getSizeItem() {
	return sizeItem;
}

bool Item::hasTileNear(glm::vec2 posTile) {
	int up = map->getTile(posTile.x, posTile.y - 1);
	int down = map->getTile(posTile.x, posTile.y + 1);
	int left = map->getTile(posTile.x - 1, posTile.y);
	int right = map->getTile(posTile.x + 1, posTile.y);
	return ((up != 0 && (up-1)/9 != 6) || (down != 0 && (down - 1) / 9 != 6)
		|| (left != 0 && (left - 1) / 9 != 6) || (right != 0 && (right - 1) / 9 != 6));
}