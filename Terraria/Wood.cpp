#include "Wood.h"

enum WoodAnims
{
	STANDBY
};

Wood::Wood(int id) {
	id_item = id;
}

void Wood::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap) {
	program = shaderProgram;
	stack = 1;
	max_stack = 99;
	map = tileMap;
	name = "Wood";
	collides = false;
	sizeItem = glm::ivec2(24, 22);
	spritesheet.loadFromFile("images/Items/Wood.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 22), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STANDBY, 1);
	sprite->addKeyframe(STANDBY, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

bool Wood::use(glm::ivec2 posMouse) {
	int tile = map->getTile(posMouse.x / 16, posMouse.y / 16);
	if (tile == 0 && hasTileNear(glm::vec2(posMouse.x / 16, posMouse.y / 16))) {
		if (isClose(posPlayer + glm::ivec2(9, 0), posPlayer + glm::ivec2(22, 32), posMouse, posMouse, 64) && !isClose(posPlayer + glm::ivec2(9, 0), posPlayer + glm::ivec2(22, 32), posMouse, posMouse, 16)) {
			map->update(glm::vec2(posMouse.x / 16, posMouse.y / 16), name);
			return true;
		}
	}
	return false;
}