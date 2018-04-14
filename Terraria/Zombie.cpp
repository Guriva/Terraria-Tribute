#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Zombie.h"
#include "Game.h"
#include <ctime>


#define JUMP_ANGLE_STEP 3
#define JUMP_HEIGHT 64
#define FALL_STEP 4

#define ATTACK_ZOMBIE_LEFT		0
#define ATTACK_ZOMBIE_RIGHT		1
#define ATTACK_ZOMBIE_UP_LEFT	2
#define ATTACK_ZOMBIE_UP_RIGHT	3
#define STATE_ZOMBIE_HIT_LEFT	4
#define STATE_ZOMBIE_HIT_RIGHT	5

enum ZombieAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT
};

ZombieAnims lastKey;


void Zombie::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	damage = 10;
	max_health = 100;
	health = 100;
	state = ATTACK_ZOMBIE_LEFT;
	cooldown = 100;
	cdHit = 15;
	lastKey = MOVE_LEFT;
	bJumping = false;
	name = "Zombie";
	sizeMonster = glm::ivec2(26, 32);
	spritesheet.loadFromFile("images/Enemies/Zombie.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(26, 32), glm::vec2(1.f, 0.1666666666666667f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STAND_LEFT, 2);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 2);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 3);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.1666666666666667f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.3333333333333333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.1666666666666667f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 3);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.6666666666666667f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.8333333333333333f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.6666666666666667f));

	sprite->setAnimationSpeed(JUMP_LEFT, 2);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f, 0.3333333333333333f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 2);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.f, 0.8333333333333333f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));

}

void Zombie::update(int deltaTime)
{
	sprite->update(deltaTime);
	//cooldown for jump
	if (cooldown < 100) ++cooldown;
	if (cdHit < 15) ++cdHit;

	if (collision(posPlayer + glm::ivec2(9, 0), glm::ivec2(13, 32), posMonster, glm::ivec2(26, 32))) {
		if (posPlayer.x + 9 > posMonster.x) hitLeft = true;
		else hitRight = true;
	}

	else {
		hitLeft = false;
		hitRight = false;
	}

	if (isHit && cdHit == 14) {
		isHit = false;
		if (lastKey == MOVE_LEFT) state = ATTACK_ZOMBIE_LEFT;
		else if (lastKey == MOVE_RIGHT) state = ATTACK_ZOMBIE_RIGHT;
		else if (lastKey == JUMP_LEFT) state = ATTACK_ZOMBIE_UP_LEFT;
		else if (lastKey == JUMP_RIGHT) state = ATTACK_ZOMBIE_UP_RIGHT;
		else if (lastKey == STAND_LEFT) state = ATTACK_ZOMBIE_LEFT;
		else if (lastKey == STAND_RIGHT) state = ATTACK_ZOMBIE_RIGHT;
	}

	if (isHit && cdHit == 15) {
		cdHit = 0;
		
	}

	//Change state depending on where is the player
	if (posPlayer.x > posMonster.x+16 && cdHit == 15) {
		state = ATTACK_ZOMBIE_RIGHT;
		if (posPlayer.y < posMonster.y && posPlayer.x - posMonster.x < 32)
			state = ATTACK_ZOMBIE_UP_RIGHT;
	}
	else if (posPlayer.x < posMonster.x-29 && cdHit == 15) {
		state = ATTACK_ZOMBIE_LEFT;
		if (posPlayer.y < posMonster.y && posMonster.x - posPlayer.x < 32)
			state = ATTACK_ZOMBIE_UP_LEFT;
	}
	
	if (state == ATTACK_ZOMBIE_LEFT || state == ATTACK_ZOMBIE_RIGHT ||
		state == ATTACK_ZOMBIE_UP_LEFT || state == ATTACK_ZOMBIE_UP_RIGHT) {
		if (state == ATTACK_ZOMBIE_LEFT || state == ATTACK_ZOMBIE_UP_LEFT)
		{
			if (state == ATTACK_ZOMBIE_LEFT) lastKey = MOVE_LEFT;
			if (state == ATTACK_ZOMBIE_UP_LEFT) lastKey = JUMP_LEFT;
			if (sprite->animation() != MOVE_LEFT && sprite->animation() != JUMP_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posMonster.x -= 1;
			if (map->collisionMoveLeft(posMonster, glm::ivec2(26, 32)))
			{
				posMonster.x += 1;
				state = ATTACK_ZOMBIE_UP_LEFT;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (state == ATTACK_ZOMBIE_RIGHT || state == ATTACK_ZOMBIE_UP_RIGHT)
		{
			if (state == ATTACK_ZOMBIE_RIGHT) lastKey = MOVE_RIGHT;
			if (state == ATTACK_ZOMBIE_UP_RIGHT) lastKey = JUMP_RIGHT;
			if (sprite->animation() != MOVE_RIGHT && sprite->animation() != JUMP_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posMonster.x += 1;
			if (map->collisionMoveRight(posMonster, glm::ivec2(26, 32)))
			{
				posMonster.x -= 1;
				state = ATTACK_ZOMBIE_UP_RIGHT;
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
			if (sprite->animation() != JUMP_LEFT && lastKey == MOVE_LEFT) {
				lastKey = JUMP_LEFT;
				sprite->changeAnimation(JUMP_LEFT);
			}
			else if (sprite->animation() != JUMP_RIGHT && lastKey == MOVE_RIGHT) {
				lastKey = JUMP_RIGHT;
				sprite->changeAnimation(JUMP_RIGHT);
			}
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posMonster.y = startY;
			}
			else
			{
				posMonster.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(posMonster + glm::ivec2(0, 0), glm::ivec2(26, 32), &posMonster.y)) bJumping = false;
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y);
			}
		}
		else
		{
			posMonster.y += FALL_STEP;
			if (map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y))
			{
				if (sprite->animation() == JUMP_LEFT || sprite->animation() == JUMP_RIGHT)
					if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
						sprite->changeAnimation(MOVE_RIGHT);
					else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
						sprite->changeAnimation(MOVE_LEFT);
					else if (lastKey == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (lastKey == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				if ((state == ATTACK_ZOMBIE_UP_LEFT || state == ATTACK_ZOMBIE_UP_RIGHT) && cooldown == 100)
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posMonster.y;
					cooldown = 0;
				}
			}
		}
	}
	else if (state == STATE_ZOMBIE_HIT_LEFT) {
		if (lastKey == MOVE_RIGHT) {
			if (cdHit < 10) {
				posMonster.x += 2;
				if (map->collisionMoveRight(posMonster, glm::ivec2(26, 32))) posMonster.x -= 2;
			}
			else {
				posMonster.x += 1;
				if (map->collisionMoveRight(posMonster, glm::ivec2(26, 32))) posMonster.x -= 1;
			}
		}
		else {
			if (cdHit < 10) {
				posMonster.x += 2;
				if (map->collisionMoveRight(posMonster, glm::ivec2(26, 32))) posMonster.x -= 2;
			}
			else {
				posMonster.x += 1;
				if (map->collisionMoveRight(posMonster, glm::ivec2(26, 32))) posMonster.x -= 1;
			}
		}
		if (bJumping)
		{
			if (sprite->animation() != JUMP_LEFT && lastKey == MOVE_LEFT)
				sprite->changeAnimation(JUMP_LEFT);
			else if (sprite->animation() != JUMP_RIGHT && lastKey == MOVE_RIGHT)
				sprite->changeAnimation(JUMP_RIGHT);
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posMonster.y = startY;
			}
			else
			{
				posMonster.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(posMonster + glm::ivec2(0, 0), glm::ivec2(26, 32), &posMonster.y)) bJumping = false;
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y);
			}
		}
		else
		{
			posMonster.y += FALL_STEP;
			if (map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y))
			{
				if (sprite->animation() == JUMP_LEFT || sprite->animation() == JUMP_RIGHT)
					if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
						sprite->changeAnimation(MOVE_RIGHT);
					else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
						sprite->changeAnimation(MOVE_LEFT);
					else if (lastKey == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (lastKey == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				if ((state == ATTACK_ZOMBIE_UP_LEFT || state == ATTACK_ZOMBIE_UP_RIGHT) && cooldown == 100)
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posMonster.y;
					cooldown = 0;
				}
			}
		}
	}

	else if (state == STATE_ZOMBIE_HIT_RIGHT) {
		if (lastKey == MOVE_LEFT) {
			if (cdHit < 10) {
				posMonster.x -= 2;
				if (map->collisionMoveLeft(posMonster, glm::ivec2(26, 32))) posMonster.x += 2;
			}
			else {
				posMonster.x -= 1;
				if (map->collisionMoveLeft(posMonster, glm::ivec2(26, 32))) posMonster.x += 1;
			}
		}
		else {
			if (cdHit < 10) {
				posMonster.x -= 2;
				if (map->collisionMoveLeft(posMonster, glm::ivec2(26, 32))) posMonster.x += 2;
			}
			else {
				posMonster.x -= 1;
				if (map->collisionMoveLeft(posMonster, glm::ivec2(26, 32))) posMonster.x += 1;
			}
		}
		if (bJumping)
		{
			if (sprite->animation() != JUMP_LEFT && lastKey == MOVE_LEFT)
				sprite->changeAnimation(JUMP_LEFT);
			else if (sprite->animation() != JUMP_RIGHT && lastKey == MOVE_RIGHT)
				sprite->changeAnimation(JUMP_RIGHT);
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posMonster.y = startY;
			}
			else
			{
				posMonster.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(posMonster + glm::ivec2(0, 0), glm::ivec2(26, 32), &posMonster.y)) bJumping = false;
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y);
			}
		}
		else
		{
			posMonster.y += FALL_STEP;
			if (map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y))
			{
				if (sprite->animation() == JUMP_LEFT || sprite->animation() == JUMP_RIGHT)
					if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
						sprite->changeAnimation(MOVE_RIGHT);
					else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
						sprite->changeAnimation(MOVE_LEFT);
					else if (lastKey == MOVE_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (lastKey == MOVE_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				if ((state == ATTACK_ZOMBIE_UP_LEFT || state == ATTACK_ZOMBIE_UP_RIGHT) && cooldown == 100)
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posMonster.y;
					cooldown = 0;
				}
			}
		}
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
}

glm::vec2 Zombie::getSize() {
	return sizeMonster;
}