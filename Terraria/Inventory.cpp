#include "Inventory.h"
#include "Game.h"
#include <iostream>

void Inventory::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 posPlayer, Sound s) {
	open = false;
	key = false;
	active = 1;
	pos = posPlayer;
	sound = s;
	program = shaderProgram;
	items = vector<Item*>(50);

	//no items at the beggining
	for (int i = 0; i < int(items.size()); ++i) {
		items[i] = nullptr;
	}

	invBack.loadFromFile("images/Layout/Inventory_Back.png", TEXTURE_PIXEL_FORMAT_RGBA);
	invBackActive.loadFromFile("images/Layout/Inventory_Back_Active.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom[2] = { glm::vec2(posPlayer.x - SCREEN_WIDTH / 2 + 16, posPlayer.y - SCREEN_HEIGHT / 2 + 32), glm::vec2(posPlayer.x - SCREEN_WIDTH / 2 + 416, posPlayer.y - SCREEN_HEIGHT / 2 + 72) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(10.f, 1.f) };
	layoutInv[0] = TexturedQuad::createTexturedQuad(geom, texCoords, shaderProgram);

	geom[0] = glm::vec2(posPlayer.x - SCREEN_WIDTH / 2 + 16, posPlayer.y - SCREEN_HEIGHT / 2 + 32);
	geom[1] = glm::vec2(posPlayer.x - SCREEN_WIDTH / 2 + 56, posPlayer.y - SCREEN_HEIGHT / 2 + 72);
	texCoords[1] = glm::vec2(1.f, 1.f);
	layoutInv[1] = TexturedQuad::createTexturedQuad(geom, texCoords, shaderProgram);

	if (!text.init("fonts/Carolina_Valtuille.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Inventory::update(glm::vec2 posPlayer) {
	pos = posPlayer;
	glm::vec2 geom[2];
	glm::vec2 texCoords[2];
	texCoords[0] = glm::vec2(0.f, 0.f);

	if (Game::instance().getKey(105) && !key) {
		if (open) sound.Play(SOUND_MENUCLOSE);
		else sound.Play(SOUND_MENUOPEN);
		open = !open;
		key = true;
	}
	if (!Game::instance().getKey(105)) key = false;
	for (int i = 48; i < 58; ++i) {
		if (Game::instance().getKey(i)) active = i - 48;
	}

	if (open) {
		geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16, pos.y - SCREEN_HEIGHT / 2 + 32);
		geom[1] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 416, pos.y - SCREEN_HEIGHT / 2 + 232);
		texCoords[1] = glm::vec2(10.f, 5.f);
		layoutInv[0] = TexturedQuad::createTexturedQuad(geom, texCoords, program);

		if (active == 0) {
			geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 376, pos.y - SCREEN_HEIGHT / 2 + 32);
			geom[1] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 416, pos.y - SCREEN_HEIGHT / 2 + 72);
			texCoords[1] = glm::vec2(1.f, 1.f);
			layoutInv[1] = TexturedQuad::createTexturedQuad(geom, texCoords, program);
		}
		else {
			geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16 + (active - 1) * 40, pos.y - SCREEN_HEIGHT / 2 + 32);
			geom[1] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16 + active * 40, pos.y - SCREEN_HEIGHT / 2 + 72);
			texCoords[1] = glm::vec2(1.f, 1.f);
			layoutInv[1] = TexturedQuad::createTexturedQuad(geom, texCoords, program);
		}
	}
	else {
		geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16, pos.y - SCREEN_HEIGHT / 2 + 32);
		geom[1] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 416, pos.y - SCREEN_HEIGHT / 2 + 72);
		texCoords[1] = glm::vec2(10.f, 1.f);
		layoutInv[0] = TexturedQuad::createTexturedQuad(geom, texCoords, program);

		if (active == 0) {
			geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 376, pos.y - SCREEN_HEIGHT / 2 + 32);
			geom[1] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 416, pos.y - SCREEN_HEIGHT / 2 + 72);
			texCoords[1] = glm::vec2(1.f, 1.f);
			layoutInv[1] = TexturedQuad::createTexturedQuad(geom, texCoords, program);
		}
		else {
			geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16 + (active - 1) * 40, pos.y - SCREEN_HEIGHT / 2 + 32);
			geom[1] = glm::vec2(pos.x	- SCREEN_WIDTH / 2 + 16 + active * 40, pos.y - SCREEN_HEIGHT / 2 + 72);
			texCoords[1] = glm::vec2(1.f, 1.f);
			layoutInv[1] = TexturedQuad::createTexturedQuad(geom, texCoords, program);
		}
	}

	for (int i = 0; i < int(items.size()); ++i) {
		if (items[i] != nullptr) {
			items[i]->setPosPlayer(pos);
			glm::vec2 sizeItem = items[i]->getSizeItem();
			int difx = int((40 - sizeItem.x) / 2);
			int dify = int((40 - sizeItem.y) / 2);
			items[i]->setPosition(glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16 + 40 * (i % 10) + difx, pos.y - SCREEN_HEIGHT / 2 + 32 + 40 * (i / 10) + dify));
		}
	}
}

void Inventory::render(glm::mat4 projection) {
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.6f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	layoutInv[0]->render(invBack);
	layoutInv[1]->render(invBackActive);
	glDisable(GL_BLEND);
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < int(items.size()); ++i) {
		if (items[i] != nullptr) {
			if (i < 10) items[i]->render();
			else if (open && i > 9) items[i]->render();
		}
	}

	for (int i = 0; i < int(items.size()); ++i) {
		if (items[i] != nullptr) {
			if (i < 10) {
				sprintf_s(textI, "%d", items[i]->getStack());
				text.render(textI, glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16 + 40 * (i % 10) + 28, pos.y - SCREEN_HEIGHT / 2 + 32 + 40 * (i / 10) + 32), 14, glm::vec4(0, 0, 0, 1), projection);
			}
			else if (open && i > 9) {
				sprintf_s(textI, "%d", items[i]->getStack());
				text.render(textI, glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16 + 40 * (i % 10) + 28, pos.y - SCREEN_HEIGHT / 2 + 32 + 40 * (i / 10) + 32), 14, glm::vec4(0, 0, 0, 1), projection);
			}
		}
	}
	
	string name = "";
	if (active == 0 && items[9] != nullptr) {
		name = items[9]->getName();
	}
	else if (active != 0 && items[active - 1] != nullptr) {
		name = items[active - 1]->getName();
	}
	strcpy_s(textI, name.c_str());
	text.render(textI, glm::vec2(pos.x - SCREEN_WIDTH / 2 + 200, pos.y - SCREEN_HEIGHT / 2 + 28), 20, glm::vec4(1, 1, 1, 1), projection);
}

int Inventory::getSize() {

	int siz = 0;
	for (int i = 0; i < int(items.size()); ++i) {
		if (items[i] != nullptr) ++siz;
	}
	return siz;
}

void Inventory::addItem(Item *it) {
	//We search for the same item in the Inventory. If it is, we look if we can add one more. If we can't, we add it in the last place if there is space.
	bool found = false;
	bool first = false;
	int pos = -1;
	int i = 0;
	while (!found && i < int(items.size())) {
		if (items[i] != nullptr && items[i]->getName() == it->getName()) {
			int num = items[i]->getStack();
			if (num < items[i]->getMaxStack()) {
				items[i]->setStack(num + 1);
				found = true;
			}
		}
		else if (items[i] == nullptr && !first) {
			first = true;
			pos = i;
		}
		++i;
	}
	if (!found && first) {
		Item* i = it->clone();
		items[pos] = i;
	}
}

bool Inventory::collectable(string name) {
	bool found = false;
	int i = 0;
	while (!found && i < int(items.size())) {
		if (items[i] != nullptr && items[i]->getName() == name) {
			int num = items[i]->getStack();
			if (num < items[i]->getMaxStack()) found = true;
		}
		else if (items[i] == nullptr) found = true;
		++i;
	}
	return found;
}

Item* Inventory::getActiveItem() {
	if (active == 0) return items[9];
	else return items[active - 1];
}

void Inventory::decrementStackActive() {
	if (active == 0 && items[9] != nullptr) {
		int num = items[9]->getStack();
		items[9]->setStack(num-1);
		if (items[9]->getStack() == 0) items[9] = nullptr;
	}
	else if (items[active - 1] != nullptr) {
		int num = items[active - 1]->getStack();
		items[active - 1]->setStack(num - 1);
		if (items[active - 1]->getStack() == 0) items[active - 1] = nullptr;
	}
}

vector<pair<int,int>> Inventory::containsItems(int num, string n) {	//First is position, second number of items
	vector<pair<int, int>> its, aux;
	int i = 0;
	int res = 0;
	bool end = false;
	while (!end && i < int(items.size())) {
		if (items[i] != nullptr && items[i]->getName() == n) {
			res += items[i]->getStack();
			its.push_back(make_pair(i, items[i]->getStack()));
		}
		if (res >= num) end = true;
		++i;
	}
	if (end) return its;
	else return aux;
}

void Inventory::deleteItem(int i) {
	items.erase(items.begin() + i);
}

void Inventory::decrementStack(int s, int pos) {
	items[pos]->setStack(items[pos]->getStack() - s);
}