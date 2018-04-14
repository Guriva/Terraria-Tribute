#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Projectile.h"
#include <ctime>

enum ProjectileAnims
{
	LAUNCH
};

#define VELOCITY 6

void Projectile::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 posMonster, glm::vec2 posPlayer)
{
	name = "Fireball";
	time = 0;
	sizeProjectile = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/Attacks/Projectile_85.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(LAUNCH, 1);
	sprite->addKeyframe(LAUNCH, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y - 16)));
	posIni = posMonster - glm::vec2(0.f,16.f);
	posFi = posPlayer;
	cout << posIni.x << " " << posIni.y << " " << posFi.x << " " << posFi.y << endl;
	posProjectile = posMonster;
}

void Projectile::update(int deltaTime)
{
	sprite->update(deltaTime);
	time += deltaTime;
	if (posIni.x == posFi.x) {
		desplx = 0;
	}
	else {
		desplx = int(VELOCITY*cos(atan(abs(float(posFi.y - posIni.y) / float(posFi.x - posIni.x)))));
	}
	if (posIni.y == posFi.y) {
		desply = 0;
	}
	else {
		desply = int(VELOCITY*sin(atan(abs(float(posFi.y - posIni.y) / float(posFi.x - posIni.x)))));
	}

	if (posFi.x < posIni.x) {
		desplx = -desplx;
	}
	if (posFi.y < posIni.y) {
		desply = -desply;
	}
	posProjectile += glm::vec2(desplx,desply);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

int Projectile::getTime() {
	return time;
}

void Projectile::render()
{
	sprite->render();
}

glm::vec2 Projectile::getPosition() {
	return posProjectile;
}