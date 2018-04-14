#include "Sword.h"
#include <iostream>
#include <cmath>

enum SwordAnims
{
	STANDBY
};

Sword::Sword(int id) {
	id_item = id;
}

void Sword::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap) {
	program = shaderProgram;
	stack = 1;
	map = tileMap;
	max_stack = 1;
	sound = 0;
	name = "Sword";
	collides = false;
	toCraft.push_back(make_pair(4, "Iron Ore"));
	toCraft.push_back(make_pair(3, "Stone"));
	sizeItem = glm::ivec2(32, 32);
	spritesheet.loadFromFile("images/Items/Sword.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STANDBY, 1);
	sprite->addKeyframe(STANDBY, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

bool Sword::use(glm::ivec2 posMouse) {
	sound = 18;
	return true;
}

int Sword::getSound() {
	return sound;
}