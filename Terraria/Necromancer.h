#pragma once
#ifndef _NECROMANCER_INCLUDE
#define _NECROMANCER_INCLUDE

#include "Monster.h"
#include "Projectile.h"

class Necromancer : public Monster
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	glm::vec2 getSize();
	bool transportLeft();
	bool transportRight();
	void renderProjectiles();
	int getSizeProjectiles();
	bool projectileHitsPlayerLeft(int j, glm::vec2 posPlayer);
	bool projectileHitsPlayerRight(int j, glm::vec2 posPlayer);
	int getTimeProjectile(int j);
	void eraseProjectile(int j);
private:
	vector<Projectile*> projectiles;
	ShaderProgram program;
};

#endif // _NECROMANCER_INCLUDE