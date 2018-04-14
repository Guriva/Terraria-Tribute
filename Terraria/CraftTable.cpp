#include "CraftTable.h"
#include "Game.h"
#include <iostream>
#include <algorithm>

void CraftTable::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 posPlayer, Inventory *inv, TileMap *tileMap) {
	inventory = inv;
	open = false;
	key = false;
	map = tileMap;
	pos = posPlayer;
	program = shaderProgram;
	items = vector<Item*>(15);

	//All items craftables
	for (int i = 0; i < int(items.size()); ++i) {
		items[i] = nullptr;
	}
	items[0] = new Pickaxe(0);
	items[0]->init(glm::ivec2(0, 0), program, map);
	items[1] = new Axe(0);
	items[1]->init(glm::ivec2(0, 0), program, map);
	items[2] = new Sword(0);
	items[2]->init(glm::ivec2(0, 0), program, map);


	invBack.loadFromFile("images/Layout/Inventory_Back2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	invBack2.loadFromFile("images/Layout/Inventory_Back3.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom[2];
	glm::vec2 texCoords[2];
	
	geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16, pos.y - SCREEN_HEIGHT / 4 + 32);
	geom[1] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 136, pos.y - SCREEN_HEIGHT / 4 + 232);
	texCoords[0] = glm::vec2(0.f, 0.f);
	texCoords[1] = glm::vec2(3.f, 5.f);

	layoutCT[0] = TexturedQuad::createTexturedQuad(geom, texCoords, program);

	texCoords[1] = glm::vec2(0.f,0.f);
	layoutCT[1] = TexturedQuad::createTexturedQuad(geom, texCoords, program);

	if (!text.init("fonts/Carolina_Valtuille.ttf"))
		cout << "Could not load font!!!" << endl;
}

void CraftTable::update(glm::vec2 posPlayer) {
	pos = posPlayer;
	glm::vec2 geom[2];
	glm::vec2 texCoords[2];
	texCoords[0] = glm::vec2(0.f, 0.f);

	if (Game::instance().getKey(105) && !key) {
		open = !open;
		key = true;
	}
	if (!Game::instance().getKey(105)) key = false;

	if (!Game::instance().getMouseKey(0)) click = false;

	if (open) {
		geom[0] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16, pos.y + 32);
		geom[1] = glm::vec2(pos.x - SCREEN_WIDTH / 2 + 136, pos.y + 232);
		texCoords[1] = glm::vec2(3.f, 5.f);
		layoutCT[0] = TexturedQuad::createTexturedQuad(geom, texCoords, program);
	}
	for (int i = 0; i < int(items.size()); ++i) {
		if (items[i] != nullptr) {
			items[i]->setPosPlayer(pos);
			glm::vec2 sizeItem = items[i]->getSizeItem();
			int difx = int((40 - sizeItem.x) / 2);
			int dify = int((40 - sizeItem.y) / 2);
			items[i]->setPosition(glm::vec2(pos.x - SCREEN_WIDTH / 2 + 16 + 40 * (i % 3) + difx, pos.y + 32 + 40 * (i / 3) + dify));
		}
	}

	int x = pos.x - SCREEN_WIDTH / 2 + 16;
	int y = pos.y + 32;
	int limx = pos.x - SCREEN_WIDTH / 2 + 136;
	int limy = pos.y + 232;
	if (posMouse.x > x && posMouse.x < limx && posMouse.y > y && posMouse.y < limy) {
		int difx = limx - x;
		int dify = limy - y;
		int xfin = (posMouse.x - x) / (difx / 3);
		int yfin = (posMouse.y - y) / (dify / 5);
		if (items[yfin * 3 + xfin] != nullptr && Game::instance().getMouseKey(0) && !click) {
			click = true;
			vector<pair<int, string>> reqItems = items[yfin * 3 + xfin]->needForCraft();
			vector<vector<pair<int, int>>> its;
			vector<bool> containItems = vector<bool>(reqItems.size(),false);
			//Check if the inventory has the items
			for (int i = 0; i < int(reqItems.size()); ++i) {
				int number = reqItems[i].first;
				string name = reqItems[i].second;
				vector<pair<int, int>> it = inventory->containsItems(number,name);
				if (it.size() > 0) {
					its.push_back(it);
					containItems[i] = true;
				}
			}
			//If it has, we delete the necessary ones and we add the item crafted to the inventory
			if (all_of(containItems.begin(), containItems.end(), [](bool b) {return b; })) {
				for (int i = 0; i < int(reqItems.size()); ++i) {
					int num = reqItems[i].first;
					string name = reqItems[i].second;
					int j = 0;
					while (j < int(its[i].size()) && num > 0) {
						if (its[i][j].second > num) {
							inventory->decrementStack(num,its[i][j].first);
							j = its[i].size();
						}
						else {
							num -= its[i][j].second;
							inventory->deleteItem(its[i][j].first);
						}
						++j;
					}
				}
				Item *i = items[yfin * 3 + xfin]->clone();
				i->init(glm::ivec2(0, 0), program, map);
				i->setPosPlayer(pos);
				inventory->addItem(i);
			}
		}
	}
}

void CraftTable::render(glm::mat4 projection) {

	if (open) {
		program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.6f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		layoutCT[0]->render(invBack);
		glDisable(GL_BLEND);
		program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < int(items.size()); ++i) {
			if (items[i] != nullptr) {
				items[i]->render();
			}
		}
		program.setUniformMatrix4f("modelview", glm::mat4(1.f));
		program.setUniform2f("texCoordDispl", 0.f, 0.f);
		//render required items window if mouse is there
		int x = pos.x - SCREEN_WIDTH / 2 + 16;
		int y = pos.y + 32;
		int limx = pos.x - SCREEN_WIDTH / 2 + 136;
		int limy = pos.y + 232;
		if (posMouse.x > x && posMouse.x < limx && posMouse.y > y && posMouse.y < limy) {
			int difx = limx - x;
			int dify = limy - y;
			int xfin = (posMouse.x - x) / (difx / 3);
			int yfin = (posMouse.y - y) / (dify / 5);
			if (items[yfin * 3 + xfin] != nullptr) {
				vector<pair<int, string>> reqItems = items[yfin * 3 + xfin]->needForCraft();
				glm::vec2 geom[2];
				glm::vec2 texCoords[2];

				geom[0] = glm::vec2(posMouse.x + 8, posMouse.y + 8);
				geom[1] = glm::vec2(posMouse.x + 138, posMouse.y + 8 + 55*(reqItems.size()));
				texCoords[0] = glm::vec2(0.f, 0.f);
				texCoords[1] = glm::vec2(1.f, 1.f);
				layoutCT[1] = TexturedQuad::createTexturedQuad(geom, texCoords, program);
				layoutCT[1]->render(invBack2);

				for (int i = 0; i < int(reqItems.size()); ++i) {
					string s = to_string(reqItems[i].first) + "x ";
					s = s + reqItems[i].second;
					strcpy_s(textI, s.c_str());
					glm::mat4 pj = projection;
					text.render(textI, glm::vec2(posMouse.x + 26, posMouse.y + 58 + 30*i), 20, glm::vec4(1, 1, 1, 1), pj);
				}
			}
		}

		string s = "Crafting";
		strcpy_s(textI, s.c_str());
		text.render(textI, glm::vec2(pos.x - SCREEN_WIDTH / 2 + 42, pos.y - 8 + 28), 20, glm::vec4(1, 1, 1, 1), projection);
	}
}

int CraftTable::getSize() {

	int siz = 0;
	for (int i = 0; i < int(items.size()); ++i) {
		if (items[i] != nullptr) ++siz;
	}
	return siz;
}