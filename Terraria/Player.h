#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Sound.h"
#include "Inventory.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

#define STATE_ACTIVE 1
#define STATE_HIT_LEFT 2
#define STATE_HIT_RIGHT 3
#define STATE_DEAD 4


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Sound s, Inventory *i);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition();
	int getMaxHealth();
	void setMaxHealth(int maxH);
	int getHealth();
	void setHealth(int H);
	bool getIsHit();
	void setIsHit(bool isH);
	int getCdHit();
	void setCdHit(int cd);
	int getState();
	void setState(int st);
	bool isDead();
	void setRespawn(const glm::vec2 &respawn);
	void addToInv(Item *it);
	bool itsCollectable(string name);
	int getSizeInventory();
	void setMousePos(glm::vec2 mPos);
	vector<Item*> itemsToUpd();
	bool isAttacking();
	string getWeapon();
	int getLastKey();
private:
	void testJump(bool bJumping, int jumpAngleStep, int jumpHeight, int fallStep);
private:
	bool bJumping, attacks;
	glm::ivec2 tileMapDispl, posPlayer, respawnPlayer, posMouse;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	string weapon;
	TileMap *map;
	int max_health, health, state, lastMove;
	int cdHit, cdRevive, cdPut, cdAttack, cdUse;
	bool isHit;
	Sound sound;
	Inventory *inventory;
	vector<Item*> items;
};


#endif // _PLAYER_INCLUDE


