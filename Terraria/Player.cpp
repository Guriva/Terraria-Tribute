#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include <ctime>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 6


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP, DEAD, DEAD2
};

PlayerAnims lastKey;


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Sound s, Inventory *i)
{
	inventory = i;
	sound = s;
	lastKey = MOVE_LEFT;
	lastMove = 1;
	bJumping = false;
	max_health = 200;
	health = 200;
	cdHit = 40;
	cdRevive = 400;
	cdPut = 7;
	cdUse = 15;
	cdAttack = 25;
	state = STATE_ACTIVE;
	spritesheet.loadFromFile("images/PJ/Isaac_All.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0833333333333333f, 0.1428571428571429f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);
	
		sprite->setAnimationSpeed(STAND_LEFT, 2);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 11.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 10.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 9.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 8.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 9.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 8.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 9.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 10.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 7.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 11.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 6.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0833333333333333f * 11.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 2);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 1.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 2.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 3.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 2.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 3.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 2.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 1.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 4.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 5.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0833333333333333f * 0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 5);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0833333333333333f * 11.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0833333333333333f * 10.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0833333333333333f * 9.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0833333333333333f * 8.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0833333333333333f * 7.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0833333333333333f * 6.f, 0.1428571428571429f * 2.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 5);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0833333333333333f * 0.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0833333333333333f * 1.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0833333333333333f * 2.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0833333333333333f * 3.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0833333333333333f * 4.f, 0.1428571428571429f * 2.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0833333333333333f * 5.f, 0.1428571428571429f * 2.f));

		sprite->setAnimationSpeed(JUMP, 4);
		sprite->addKeyframe(JUMP, glm::vec2(0.0833333333333333f * 5.f, 0.1428571428571429f * 5.f));
		sprite->addKeyframe(JUMP, glm::vec2(0.0833333333333333f * 6.f, 0.1428571428571429f * 5.f));

		sprite->setAnimationSpeed(DEAD, 1);
		sprite->addKeyframe(DEAD, glm::vec2(0.0833333333333333f * 2.f, 0.1428571428571429f * 5.f));

		sprite->setAnimationSpeed(DEAD2, 2);
		sprite->addKeyframe(DEAD2, glm::vec2(0.0833333333333333f * 9.f, 0.1428571428571429f * 5.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	
	//cooldowns
	if (cdHit < 40) ++cdHit;
	if (cdRevive < 399) ++cdRevive;
	if (cdPut < 7) ++cdPut;
	if (cdUse < 15) ++cdUse;
	if (cdAttack < 25) ++cdAttack;

	if (cdRevive == 399) {
		++cdRevive;
		health = max_health;
		cdRevive = 400;
		posPlayer = respawnPlayer;
		state = STATE_ACTIVE;
		sprite->changeAnimation(STAND_LEFT);
		isHit = false;
	}

	if (state == STATE_DEAD && cdRevive == 400) {
		cdRevive = 0;
		sound.Play(SOUND_PLAYERKILLED);
	}

	if (isHit && cdHit == 40 && state != STATE_DEAD) {
		sound.Play(SOUND_PLAYERHIT);
		cdHit = 0;
	}
	if (isHit && cdHit > 15 && state != STATE_DEAD) {
		isHit = false;
		state = STATE_ACTIVE;
	}

	if (cdAttack > 8 && attacks) attacks = false;

	if (Game::instance().getMouseKey(0) && state != STATE_DEAD) {
		Item *it = inventory->getActiveItem();
		if (it != nullptr) {
			string obj = it->getName();
			if ((obj == "Dirt" || obj == "Iron Ore" || obj == "Gold Ore" || obj == "Stone" || obj == "Wood") && cdPut == 7) {
				bool done = it->use(posMouse);
				if (done) {
					cdPut = 0;
					sound.Play(SOUND_PUT);
					inventory->decrementStackActive();
				}
			}
			else if (obj == "Pickaxe" && cdUse == 15) {
				bool done = it->use(posMouse);
				if (done) {
					int s = it->getSound();
					cdUse = 0;
					sound.Play(s);
					items = it->itemsToAdd();
				}
			}
			else if (obj == "Axe" && cdUse == 15) {
				bool done = it->use(posMouse);
				if (done) {
					int s = it->getSound();
					cdUse = 0;
					sound.Play(s);
					items = it->itemsToAdd();
				}
			}
			else if (obj == "Sword" && cdAttack == 25) {
				bool done = it->use(posMouse);
				if (done) {
					int s = it->getSound();
					cdAttack = 0;
					sound.Play(s);
					attacks = true;
					weapon = it->getName();
				}
			}
		}
	}

	if (state == STATE_ACTIVE) {
		if (Game::instance().getKey(97))
		{
			lastKey = MOVE_LEFT;
			lastMove = 1;
			if (sprite->animation() != MOVE_LEFT && sprite->animation() != JUMP)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32)))
			{
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getKey(100))
		{
			lastKey = MOVE_RIGHT;
			lastMove = 2;
			if (sprite->animation() != MOVE_RIGHT && sprite->animation() != JUMP)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}

		if (bJumping)
		{
			if (sprite->animation() != JUMP)
				sprite->changeAnimation(JUMP);
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else {
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y)) bJumping = false;
				if (jumpAngle > 90) {
					bJumping = !map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y);
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y))
			{
				if (sprite->animation() == JUMP)
					if (Game::instance().getKey(100))
						sprite->changeAnimation(MOVE_RIGHT);
					else if (Game::instance().getKey(97))
						sprite->changeAnimation(MOVE_LEFT);
					else if (lastKey == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (lastKey == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				if (Game::instance().getKey(32))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
	}
	else if (state == STATE_HIT_LEFT) {
		if (lastKey == MOVE_RIGHT) {
			if (cdHit < 10) {
				posPlayer.x += 4;
				if (map->collisionMoveRight(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x -= 4;
			}
			else {
				posPlayer.x += 2;
				if (map->collisionMoveRight(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x -= 2;
			}
		}
		else {
			if (cdHit < 10) {
				posPlayer.x += 3;
				if (map->collisionMoveRight(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x -= 3;
			}
			else {
				posPlayer.x += 2;
				if (map->collisionMoveRight(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x -= 2;
			}
		}
		if (bJumping)
		{
			if (sprite->animation() != JUMP)
				sprite->changeAnimation(JUMP);
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else {
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y)) bJumping = false;
				if (jumpAngle > 90) {
					bJumping = !map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y);
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y))
			{
				if (sprite->animation() == JUMP)
					if (Game::instance().getKey(100))
						sprite->changeAnimation(MOVE_RIGHT);
					else if (Game::instance().getKey(97))
						sprite->changeAnimation(MOVE_LEFT);
					else if (lastKey == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (lastKey == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				if (Game::instance().getKey(32))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
	}

	else if (state == STATE_HIT_RIGHT) {
		if (lastKey == MOVE_LEFT) {
			if (cdHit < 10) {
				posPlayer.x -= 4;
				if (map->collisionMoveLeft(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x += 4;
			}
			else {
				posPlayer.x -= 2;
				if (map->collisionMoveLeft(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x += 2;
			}
		}
		else {
			if (cdHit < 10) {
				posPlayer.x -= 3;
				if (map->collisionMoveLeft(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x += 3;
			}
			else {
				posPlayer.x -= 2;
				if (map->collisionMoveLeft(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32))) posPlayer.x += 2;
			}
		}
		if (bJumping)
		{
			if (sprite->animation() != JUMP)
				sprite->changeAnimation(JUMP);
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else {
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y)) bJumping = false;
				if (jumpAngle > 90) {
					bJumping = !map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y);
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y))
			{
				if (sprite->animation() == JUMP)
					if (Game::instance().getKey(100))
						sprite->changeAnimation(MOVE_RIGHT);
					else if (Game::instance().getKey(97))
						sprite->changeAnimation(MOVE_LEFT);
					else if (lastKey == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (lastKey == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				if (Game::instance().getKey(32))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
	}

	if (state == STATE_DEAD) {
		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else {
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y)) bJumping = false;
				if (jumpAngle > 90) {
					bJumping = !map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y);
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), &posPlayer.y))
			{
				if (lastKey == MOVE_LEFT) sprite->changeAnimation(DEAD2);
				else if (lastKey == MOVE_RIGHT) sprite->changeAnimation(DEAD);
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setRespawn(const glm::vec2 &respawn) {
	respawnPlayer = respawn;
}

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

int Player::getMaxHealth() {
	return max_health;
}

void Player::setMaxHealth(int maxH) {
	max_health = maxH;
}

int Player::getHealth() {
	return health;
}

void Player::setHealth(int H) {
	if (H < 0) health = 0;
	else health = H;
}

bool Player::getIsHit() {
	return isHit;
}
void Player::setIsHit(bool isH) {
	isHit = isH;
}

int Player::getCdHit() {
	return cdHit;
}

void Player::setCdHit(int cd) {
	cdHit = cd;
}

int Player::getState() {
	return state;
}

void Player::setState(int st) {
	state = st;
}

void Player::testJump(bool bJump, int jumpAngleStep, int jumpHeight, int fallStep) {
	bJumping = bJump;
	
}

bool Player::isDead() {
	return health == 0;
}

int Player::getSizeInventory() {
	return inventory->getSize();
}

void Player::addToInv(Item *it) {
	inventory->addItem(it);
}

bool Player::itsCollectable(string name) {
	return inventory->collectable(name);
}

void Player::setMousePos(glm::vec2 mPos) {
	posMouse = mPos;
}

vector<Item*> Player::itemsToUpd() {
	vector<Item*> itt;
	for (int i = 0; i < int(items.size()); ++i) {
		Item *it = items[i]->clone();
		itt.push_back(it);
	}
	items.clear();
	return itt;
}

bool Player::isAttacking() {
	return attacks;
}

string Player::getWeapon() {
	return weapon;
}

int Player::getLastKey() {
	return lastMove;
}