#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Monster.h"
#include "Game.h"
#include <ctime>

void Monster::render()
{
	sprite->render();
}

void Monster::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Monster::setPosition(const glm::vec2 &pos)
{
	posMonster = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
}

glm::vec2 Monster::getPosition()
{
	return posMonster;
}

int Monster::getMaxHealth() {
	return max_health;
}

void Monster::setMaxHealth(int maxH) {
	max_health = maxH;
}

int Monster::getHealth() {
	return health;
}

void Monster::setHealth(int H) {
	health = H;
}

void Monster::setState(int st) {
	state = st;
}

int Monster::getState() {
	return state;
}

void Monster::setPosPlayer(glm::ivec2 posP) {
	posPlayer = posP;
}

glm::vec2 Monster::getPosPlayer() {
	return posPlayer;
}

bool Monster::collision(glm::ivec2 posPlayer, glm::ivec2 sizePlayer, glm::ivec2 posMonster, glm::ivec2 sizeMonster) {
	if (posPlayer.x < posMonster.x + sizeMonster.x &&
		posPlayer.x + sizePlayer.x > posMonster.x &&
		posPlayer.y < posMonster.y + sizeMonster.y &&
		sizePlayer.y + posPlayer.y > posMonster.y) return true;
	return false;
}

bool Monster::getHitLeft() {
	return hitLeft;
}
void Monster::setHitLeft(bool HL) {
	hitLeft = HL;
}
bool Monster::getHitRight() {
	return hitRight;
}
void Monster::setHitRight(bool HR) {
	hitRight = HR;
}

int Monster::getDamage() {
	return damage;
}
void Monster::setDamage(int dmg) {
	damage = dmg;
}
