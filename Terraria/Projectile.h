#pragma once
#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Inventory.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

#define STATE_ACTIVE 1
#define STATE_HIT_LEFT 2
#define STATE_HIT_RIGHT 3
#define STATE_DEAD 4


class Projectile
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 posMonster, glm::vec2 posPlayer);
	void update(int deltaTime);
	glm::ivec2 getSizeProjectile() { return sizeProjectile; }
	void render();
	int getTime();
	glm::vec2 getPosition();
private:
	glm::ivec2 sizeProjectile, tileMapDispl, posProjectile, posIni, posFi;
	int time;
	int desplx, desply;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	string name;
};

#endif // _PROJECTILE_INCLUDE