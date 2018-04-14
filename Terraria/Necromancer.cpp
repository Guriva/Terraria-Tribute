#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Necromancer.h"
#include "Game.h"
#include <ctime>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ATTACK_NECRO_LEFT		0
#define ATTACK_NECRO_RIGHT		1
#define STAND_NECRO_LEFT		2
#define STAND_NECRO_RIGHT		3
#define TRANSPORT_NECRO			4


enum NecromancerAnims
{
	STAND_LEFT, STAND_RIGHT, ATTACK_LEFT, ATTACK_RIGHT
};

NecromancerAnims lastKey;


void Necromancer::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	damage = 10;
	max_health = 200;
	health = 200;
	cooldown = 400;
	cdHit = 15;
	state = STAND_NECRO_LEFT;
	lastKey = STAND_LEFT;
	program = shaderProgram;
	name = "Necromancer";
	sizeMonster = glm::ivec2(26, 32);
	spritesheet.loadFromFile("images/Enemies/Necromancer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(26, 32), glm::vec2(1.f, 0.1666666666666667f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 2);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 2);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 3);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.f, 0.1666666666666667f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.f, 0.3333333333333333f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.f, 0.1666666666666667f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 3);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.f, 0.6666666666666667f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.f, 0.8333333333333333f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.f, 0.6666666666666667f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));

}

void Necromancer::update(int deltaTime)
{
	sprite->update(deltaTime);
	
	for (int i = 0; i < int(projectiles.size()); ++i) {
		projectiles[i]->update(deltaTime);
		if (projectiles[i]->getTime() > 4000) eraseProjectile(i);
	}

	if (cooldown < 400) ++cooldown;
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
	}

	if (isHit && cdHit == 15) {
		cdHit = 0;

	}

	
	if (posPlayer.x > posMonster.x && posPlayer.x <= posMonster.x + 500 && posPlayer.y >= posMonster.y - 250 && posPlayer.y <= posMonster.y + 250) {
		state = ATTACK_NECRO_RIGHT;
	}
	else if (posPlayer.x < posMonster.x && posPlayer.x >= posMonster.x - 500 && posPlayer.y >= posMonster.y - 250 && posPlayer.y <= posMonster.y + 250) {
		state = ATTACK_NECRO_LEFT;
	}
	else if (posPlayer.x > posMonster.x + 500 || posPlayer.y > posMonster.y + 500) {
		state = STAND_NECRO_RIGHT;
	}
	else if (posPlayer.x < posMonster.x - 500 || posPlayer.y < posMonster.y - 500) {
		state = STAND_NECRO_LEFT;
	}

	//Change state depending on where is the player
	if (state == STAND_NECRO_LEFT) {
		lastKey = STAND_LEFT;
		if (sprite->animation() != STAND_LEFT) sprite->changeAnimation(STAND_LEFT);
		posMonster.y += FALL_STEP;
		map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y);
	}
	else if (state == STAND_NECRO_RIGHT) {
		lastKey = STAND_RIGHT;
		if (sprite->animation() != STAND_RIGHT) sprite->changeAnimation(STAND_RIGHT);
		posMonster.y += FALL_STEP;
		map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y);
	}
	else if (state == ATTACK_NECRO_LEFT) {
		if (cooldown < 250) {
			if (lastKey == ATTACK_RIGHT || lastKey == STAND_RIGHT) sprite->changeAnimation(STAND_LEFT);
		}
		lastKey = ATTACK_LEFT;
		if (cooldown == 250) {
			transportLeft();
			sprite->changeAnimation(ATTACK_LEFT);
		}
		else if (cooldown == 300) {
			Projectile* p = new Projectile();
			p->init(tileMapDispl, program, posMonster, posPlayer);
			projectiles.push_back(p);
		}
		else if (cooldown == 350) {
			Projectile* p = new Projectile();
			p->init(tileMapDispl, program, posMonster, posPlayer);
			projectiles.push_back(p);
		}
		else if (cooldown == 400) {
			Projectile* p = new Projectile();
			p->init(tileMapDispl, program, posMonster, posPlayer);
			projectiles.push_back(p);
			cooldown = 0;
			sprite->changeAnimation(STAND_LEFT);
		}
		
		posMonster.y += FALL_STEP;
		map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y);

	}
	else if (state == ATTACK_NECRO_RIGHT) {
		if (cooldown < 250) {
			if (lastKey == ATTACK_LEFT || lastKey == STAND_LEFT) sprite->changeAnimation(STAND_RIGHT);
		}
		lastKey = ATTACK_RIGHT;
		if (cooldown == 250) {
			transportRight();
			sprite->changeAnimation(ATTACK_RIGHT);
		}
		else if (cooldown == 300) {
			Projectile* p = new Projectile();
			p->init(tileMapDispl, program, posMonster, posPlayer);
			projectiles.push_back(p);
		}
		else if (cooldown == 350) {
			Projectile* p = new Projectile();
			p->init(tileMapDispl, program, posMonster, posPlayer);
			projectiles.push_back(p);
		}
		else if (cooldown == 400) {
			Projectile* p = new Projectile();
			p->init(tileMapDispl, program, posMonster, posPlayer);
			projectiles.push_back(p);
			cooldown = 0;
			sprite->changeAnimation(STAND_RIGHT);
		}
		posMonster.y += FALL_STEP;
		map->collisionMoveDown(posMonster, glm::ivec2(26, 32), &posMonster.y);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
}

glm::vec2 Necromancer::getSize() {
	return sizeMonster;
}

bool Necromancer::transportLeft() {

	int posx = posMonster.x;
	int posy = posMonster.y;
	bool found = false;
	for (int x = posx - 208; x <= posx && !found; x += 16) {
		for (int y = posy - 88; y <= posy && !found; y += 16) {
			if (!map->collisionNecro(glm::vec2(x, y), glm::vec2(26.f, 32.f))) {
				posMonster = glm::vec2(x, y);
				found = true;
			}
		}
	}

	for (int x = posx - 208; x <= posx && !found; x += 16) {
		for (int y = posy + 88; y >= posy && !found; y -= 16) {
			if (!map->collisionNecro(glm::vec2(x, y), glm::vec2(26.f, 32.f))) {
				posMonster = glm::vec2(x, y);
				found = true;
			}
		}
	}
	return found;
}

bool Necromancer::transportRight() {
	int posx = posMonster.x;
	int posy = posMonster.y;
	bool found = false;
	for (int x = posx + 208; x >= posx && !found; x -= 16) {
		for (int y = posy - 88; y <= posy && !found; y += 16) {
			if (!map->collisionNecro(glm::vec2(x, y), glm::vec2(26.f, 32.f))) {
				posMonster = glm::vec2(x, y);
				found = true;
			}
		}
	}

	for (int x = posx + 208; x >= posx && !found; x -= 16) {
		for (int y = posy + 88; y >= posy && !found; y -= 16) {
			if (!map->collisionNecro(glm::vec2(x, y), glm::vec2(26.f, 32.f))) {
				posMonster = glm::vec2(x, y);
				found = true;
			}
		}
	}

	return found;
}

void Necromancer::renderProjectiles() {
	for (int i = 0; i < int(projectiles.size()); ++i) {
		projectiles[i]->render();
	}
}

int Necromancer::getSizeProjectiles() {
	return projectiles.size();
}

bool Necromancer::projectileHitsPlayerLeft(int j, glm::vec2 posPlayer) {
	glm::vec2 posProj = projectiles[j]->getPosition();
	return (collision(posProj, glm::vec2(16.f, 16.f), posPlayer + glm::vec2(9, 0), glm::ivec2(13, 32)) && posProj.x < posPlayer.x);
}
bool Necromancer::projectileHitsPlayerRight(int j, glm::vec2 posPlayer) {
	glm::vec2 posProj = projectiles[j]->getPosition();
	return (collision(posProj, glm::vec2(16.f, 16.f), posPlayer + glm::vec2(9, 0), glm::ivec2(13, 32)) && posProj.x > posPlayer.x);
}

int Necromancer::getTimeProjectile(int j) {
	return projectiles[j]->getTime();
}

void Necromancer::eraseProjectile(int j) {
	projectiles.erase(projectiles.begin()+j);
}