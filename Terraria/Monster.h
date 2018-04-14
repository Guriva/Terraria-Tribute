#pragma once
#ifndef _MONSTER_INCLUDE
#define _MONSTER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Projectile.h"

class Monster
{

public:
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition();
	int getMaxHealth();
	void setMaxHealth(int maxH);
	int getHealth();
	void setHealth(int H);
	void setState(int st);
	int getState();
	void setPosPlayer(glm::ivec2 posP);
	bool getHitLeft();
	void setHitLeft(bool HL);
	bool getHitRight();
	void setHitRight(bool HR);
	int getDamage();
	void setDamage(int dmg);
	glm::vec2 getPosPlayer();
	bool collision(glm::ivec2 posPlayer, glm::ivec2 sizePlayer, glm::ivec2 posMonster, glm::ivec2 sizeMonster);
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	virtual glm::vec2 getSize() = 0;
	virtual void renderProjectiles() {}
	virtual int getSizeProjectiles() { return 0; }
	virtual bool projectileHitsPlayerLeft(int j, glm::vec2 posPlayer) { return false; }
	virtual bool projectileHitsPlayerRight(int j, glm::vec2 posPlayer) { return false; }
	virtual int getTimeProjectile(int j) { return 0; }
	virtual void eraseProjectile(int j) {}
	void setIsHit(bool b) { isHit = b; }
	int getCdHit() { return cdHit; }
	string getName() { return name; }
protected:
	bool bJumping, hitLeft, hitRight, isHit;
	glm::ivec2 tileMapDispl, posMonster, posPlayer, sizeMonster;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	string name;
	int max_health, health, state, cooldown, damage;
	int cdHit;
};
#endif // _MONSTER_INCLUDE