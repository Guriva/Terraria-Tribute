#include <GL/glew.h>
#include <GL/glut.h>
#include "Mouse.h"
#include "Game.h"
#include <iostream>

int posMousex, posMousey;

static void position(int x1, int y1) {
	posMousex = x1;
	posMousey = y1;
}

void Mouse::changeMouseSprite(const glm::vec2 pos) {
	program.use();
	glutSetCursor(GLUT_CURSOR_NONE);
	glutPassiveMotionFunc(position);
	glutMotionFunc(position);
	float posx = float(posMousex + pos.x - float(SCREEN_WIDTH) / 2 + float(32 * posMousex / (SCREEN_WIDTH - 1)));
	float posy = float(posMousey + pos.y - float(SCREEN_HEIGHT) / 2 + float(32 * posMousey / (SCREEN_HEIGHT - 1)));
	spriteMouse->setPosition(glm::vec2(posx, posy));
	spriteMouse->render();
}

void Mouse::changeMouseSpriteMenu() {
	program.use();
	glutSetCursor(GLUT_CURSOR_NONE);
	glutPassiveMotionFunc(position);
	glutMotionFunc(position);
	spriteMouse->setPosition(glm::vec2(posMousex, posMousey));
	spriteMouse->render();
}

void Mouse::createMouse(ShaderProgram &texProgram, const glm::vec2 &pos) {
	program = texProgram;
	spritesheetMouse.loadFromFile("images/Items/Cursor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteMouse = Sprite::createSprite(glm::ivec2(14, 14), glm::vec2(1.f, 1.f), &spritesheetMouse, &texProgram);
	glutPassiveMotionFunc(position);
	glutMotionFunc(position);
	spriteMouse->setPosition(glm::vec2(float(pos.x - 1024 + posMousex), float(pos.y - 768 + posMousey)));
}

glm::vec2 Mouse::getMousePos(const glm::vec2 pos) {
	float posx = float(posMousex + pos.x - float(SCREEN_WIDTH) / 2 + float(32 * posMousex / (SCREEN_WIDTH - 1)));
	float posy = float(posMousey + pos.y - float(SCREEN_HEIGHT) / 2 + float(32 * posMousey / (SCREEN_HEIGHT - 1)));
	return glm::vec2(posx, posy);
}

glm::vec2 Mouse::getMousePosMenu() {
	return glm::vec2(posMousex, posMousey);
}

void Mouse::setMousePos(int x, int y) {
	posMousex = x;
	posMousey = y;
}