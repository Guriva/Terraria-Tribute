#include "Pickaxe.h"
#include "Dirt.h"
#include "IronOre.h"
#include "GoldOre.h"
#include "Stone.h"
#include "Wood.h"
#include <iostream>
#include <cmath>

enum PickaxeAnims
{
	STANDBY
};

Pickaxe::Pickaxe(int id) {
	id_item = id;
}

void Pickaxe::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap) {
	program = shaderProgram;
	stack = 1;
	map = tileMap;
	max_stack = 1;
	sound = 0;
	name = "Pickaxe";
	collides = false;
	toCraft.push_back(make_pair(5, "Wood"));
	toCraft.push_back(make_pair(2, "Stone"));
	sizeItem = glm::ivec2(32, 32);
	spritesheet.loadFromFile("images/Items/Pick.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STANDBY, 1);
	sprite->addKeyframe(STANDBY, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

bool Pickaxe::use(glm::ivec2 posMouse) {

	int tile = map->getTile(posMouse.x / 16, posMouse.y / 16);
	if (tile != 0 && isClose(posPlayer + glm::ivec2(9, 0), posPlayer + glm::ivec2(22, 32), posMouse, posMouse, 64)
		&& !isClose(posPlayer + glm::ivec2(9, 0), posPlayer + glm::ivec2(22, 32), posMouse, posMouse, 16)) {
		if (tile > 0 && tile < 28) {		//Dirt
			sound = 17;
			int posx = posMouse.x / 16;
			int posy = posMouse.y / 16;
			map->update(glm::vec2(posx, posy), name);
			Item* it = new Dirt(0);
			it->init(glm::ivec2(0,0),program,map);
			it->setPosition(glm::vec2(posx*16,posy*16));
			it->setTileMap(map);
			it->setPosPlayer(posPlayer);
			itemsQueue.push_back(it);
			return true;
		}
		else if (tile > 27 && tile < 46) {	//Stone
			sound = 11;
			int posx = posMouse.x / 16;
			int posy = posMouse.y / 16;
			map->update(glm::vec2(posx, posy), name);
			Item* it = new Stone(0);
			it->init(glm::ivec2(0, 0), program, map);
			it->setPosition(glm::vec2(posx*16, posy*16));
			it->setTileMap(map);
			it->setPosPlayer(posPlayer);
			itemsQueue.push_back(it);
			return true;
		}
		else if (tile > 45 && tile < 55) {	//Wood
			sound = 19;
			map->update(glm::vec2(posMouse.x / 16, posMouse.y / 16), name);
			Item* it = new Wood(0);
			it->init(glm::ivec2(0, 0), program, map);
			it->setPosition(glm::vec2(posMouse.x, posMouse.y));
			it->setTileMap(map);
			it->setPosPlayer(posPlayer);
			itemsQueue.push_back(it);
			return true;
		}
		else if (tile  > 63 && tile < 82) {	//Gold Ore
			sound = 11;
			map->update(glm::vec2(posMouse.x / 16, posMouse.y / 16), name);
			Item* it = new GoldOre(0);
			it->init(glm::ivec2(0, 0), program, map);
			it->setPosition(glm::vec2(posMouse.x, posMouse.y));
			it->setTileMap(map);
			it->setPosPlayer(posPlayer);
			itemsQueue.push_back(it);
			return true;
		}
		else if (tile > 81 && tile < 100) {	//Iron Ore
			sound = 11;
			map->update(glm::vec2(posMouse.x / 16, posMouse.y / 16), name);
			Item* it = new IronOre(0);
			it->init(glm::ivec2(0, 0), program, map);
			it->setPosition(glm::vec2(posMouse.x, posMouse.y));
			it->setTileMap(map);
			it->setPosPlayer(posPlayer);
			itemsQueue.push_back(it);
			return true;
		}
	}
	return false;
}

vector<Item*> Pickaxe::itemsToAdd() {
	vector<Item*> v;
	for (int i = 0; i < itemsQueue.size(); ++i) {
		Item* it = itemsQueue[i]->clone();
		v.push_back(it);
	}
	itemsQueue.clear();
	return v;
}

int Pickaxe::getSound() {
	return sound;
}