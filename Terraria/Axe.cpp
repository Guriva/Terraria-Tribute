#include "Axe.h"
#include "Wood.h"
#include <iostream>
#include <cmath>

enum AxeAnims
{
	STANDBY
};

Axe::Axe(int id) {
	id_item = id;
}

void Axe::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap) {
	program = shaderProgram;
	stack = 1;
	map = tileMap;
	max_stack = 1;
	sound = 0;
	name = "Axe";
	collides = false;
	toCraft.push_back(make_pair(4,"Wood"));
	toCraft.push_back(make_pair(2, "Stone"));
	sizeItem = glm::ivec2(32, 32);
	spritesheet.loadFromFile("images/Items/Axe.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STANDBY, 1);
	sprite->addKeyframe(STANDBY, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

bool Axe::use(glm::ivec2 posMouse) {
	bool found = false;
	int tile = map->getTile(posMouse.x / 16, posMouse.y / 16);
	if (tile != 0 && isClose(posPlayer + glm::ivec2(9, 0), posPlayer + glm::ivec2(22, 32), posMouse, posMouse, 64)
		&& !isClose(posPlayer + glm::ivec2(9, 0), posPlayer + glm::ivec2(22, 32), posMouse, posMouse, 16)) {
		int posx = posMouse.x / 16;
		int posy = posMouse.y / 16;
		if (tile > 54 && tile < 64) {		//Tree
			found = true;
			sound = 19;
			map->update(glm::vec2(posx, posy), name);
			Item* it = new Wood(0);
			it->init(glm::ivec2(0, 0), program, map);
			it->setPosition(glm::vec2(posx * 16, posy * 16));
			it->setTileMap(map);
			it->setPosPlayer(posPlayer);
			itemsQueue.push_back(it);
		}
		if (found) {
			--posy;
			tile = map->getTile(posx, posy);
			while (tile > 54 && tile < 64) {
				map->update(glm::vec2(posx, posy), name);
				Item* it = new Wood(0);
				it->init(glm::ivec2(0, 0), program, map);
				it->setPosition(glm::vec2(posx * 16, posy * 16));
				it->setTileMap(map);
				it->setPosPlayer(posPlayer);
				itemsQueue.push_back(it);
				--posy;
				tile = map->getTile(posx, posy);
			}
			if (tile > 99) {	//Delete the upper tree if there is
				map->update(glm::vec2(posx-2, posy-4), name); map->update(glm::vec2(posx-1, posy-4), name); map->update(glm::vec2(posx, posy-4), name);
				map->update(glm::vec2(posx+1, posy-4), name); map->update(glm::vec2(posx+2, posy-4), name); map->update(glm::vec2(posx-2, posy-3), name);
				map->update(glm::vec2(posx-1, posy-3), name); map->update(glm::vec2(posx, posy-3), name); map->update(glm::vec2(posx+1, posy-3), name);
				map->update(glm::vec2(posx+2, posy-3), name); map->update(glm::vec2(posx-2, posy-2), name); map->update(glm::vec2(posx-1, posy-2), name);
				map->update(glm::vec2(posx, posy-2), name); map->update(glm::vec2(posx+1, posy-2), name); map->update(glm::vec2(posx+2, posy-2), name);
				map->update(glm::vec2(posx-2, posy-1), name); map->update(glm::vec2(posx-1, posy-1), name); map->update(glm::vec2(posx, posy-1), name);
				map->update(glm::vec2(posx+1, posy-1), name); map->update(glm::vec2(posx+2, posy-1), name); map->update(glm::vec2(posx-2, posy), name);
				map->update(glm::vec2(posx-1, posy), name); map->update(glm::vec2(posx, posy), name); map->update(glm::vec2(posx+1, posy), name);
				map->update(glm::vec2(posx+2, posy), name);

			}
		}
	}
	return found;
}

vector<Item*> Axe::itemsToAdd() {
	vector<Item*> v;
	for (int i = 0; i < itemsQueue.size(); ++i) {
		Item* it = itemsQueue[i]->clone();
		v.push_back(it);
	}
	itemsQueue.clear();
	return v;
}

int Axe::getSound() {
	return sound;
}