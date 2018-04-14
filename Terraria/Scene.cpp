#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Scene.h"
#include "Game.h"

#define PLAY	0
#define MAIN	1
#define HOWTO	2
#define CREDITS	3

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 120
#define INIT_PLAYER_Y_TILES 10

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	window = MAIN;
	Init = false;
	initShaders();
	sound.Load();
	sound.Play(SOUND_TITLESCREEN);
	mouse = new Mouse();
	mouse->createMouse(texProgram, glm::vec2(0, 0));
	textsMenu[0].loadFromFile("images/Layout/Inicial.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[1].loadFromFile("images/Layout/How_to_play.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[2].loadFromFile("images/Layout/Credits.png", TEXTURE_PIXEL_FORMAT_RGB);
	textsMenu[3].loadFromFile("images/Layout/Back.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[4].loadFromFile("images/Layout/Back_Active.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[5].loadFromFile("images/Layout/Play.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[6].loadFromFile("images/Layout/Play_Active.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[7].loadFromFile("images/Layout/How_to_play_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[8].loadFromFile("images/Layout/How_to_play_button_Active.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[9].loadFromFile("images/Layout/Credits_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textsMenu[10].loadFromFile("images/Layout/Credits_button_Active.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Scene::initGame() {
	loadBackgrounds();
	map = TileMap::createTileMap("levels/Mapa2.txt", glm::vec2(SCREEN_X, SCREEN_Y), &texProgram);
	sound.StopAll();
	sound.Play(SOUND_AMBIENT1);
	inventory = new Inventory();
	craftTable = new CraftTable();
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, sound, inventory);
	inventory->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player->getPosition(), sound);
	craftTable->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player->getPosition(), inventory, map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setRespawn(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	Item *i;
	i = new Wood(1);
	i->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	i->setStack(10);
	player->addToInv(i);
	i = new Stone(1);
	i->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	i->setStack(10);
	player->addToInv(i);
	loadLayouts();
	initMonsters();		//Only if map 2! If you're playing map 1, comment this line.
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	if (!text.init("fonts/Carolina_Valtuille.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::update(int deltaTime)
{
	if (window != 0) {
		currentTime += deltaTime;
		glm::vec2 pMouse = mouse->getMousePosMenu();

		if (window == MAIN) {
			if (pMouse.x > 500 && pMouse.x < 565 && pMouse.y > 250 && pMouse.y < 299 && Game::instance().getMouseKey(0)) window = PLAY;
			else if (pMouse.x > 426 && pMouse.x < 633 && pMouse.y > 320 && pMouse.y < 366 && Game::instance().getMouseKey(0)) window = HOWTO;
			else if (pMouse.x > 470 && pMouse.x < 593 && pMouse.y > 390 && pMouse.y < 426 && Game::instance().getMouseKey(0)) window = CREDITS;
		}
		else if (window == HOWTO) {
			if (pMouse.x > 400 && pMouse.x < 616 && pMouse.y > 650 && pMouse.y < 720 && Game::instance().getMouseKey(0)) window = MAIN;
		}
		else if (window == CREDITS) {
			if (pMouse.x > 400 && pMouse.x < 616 && pMouse.y > 650 && pMouse.y < 720 && Game::instance().getMouseKey(0)) window = MAIN;
		}

	}
	else if (window == PLAY) {
		currentTime += deltaTime;
		updateBackgrounds();
		player->update(deltaTime);							//update player
		vector<Item*> its = player->itemsToUpd();
		for (int i = 0; i < int(its.size()); ++i) items.push_back(its[i]);
		glm::vec2 posMouse = mouse->getMousePos(player->getPosition());
		player->setMousePos(posMouse);
		craftTable->setMousePos(posMouse);
		glm::vec2 pos = player->getPosition();
		setAllStates();										//update position of player for all the objects
		for (int i = 0; i < int(monsters.size()) && monsters[i] != NULL; ++i) {
			bool dead = false;
			monsters[i]->update(deltaTime);					//update monsters
			if (player->isAttacking()) {
				int side = player->getLastKey();
				string weapon = player->getWeapon();
				if (weapon == "Sword") {
					if (side == 1 && monsters[i]->collision(glm::ivec2(pos.x - 18, pos.y + 2), glm::ivec2(26, 26), monsters[i]->getPosition(), monsters[i]->getSize()) && monsters[i]->getCdHit() == 15) {
						monsters[i]->setIsHit(true);
						monsters[i]->setState(5);
						monsters[i]->setHealth(monsters[i]->getHealth() - 15);		//Damage by default 15
						if (monsters[i]->getHealth() < 0) {
							sound.Play(SOUND_MONSTERKILLED);
							monsters.erase(monsters.begin() + i);
							dead = true;
						}
						else sound.Play(SOUND_NPCHIT);
					}
					else if (side == 2 && monsters[i]->collision(glm::ivec2(pos.x + 26, pos.y + 2), glm::ivec2(26, 26), monsters[i]->getPosition(), monsters[i]->getSize()) && monsters[i]->getCdHit() == 15) {
						monsters[i]->setIsHit(true);
						monsters[i]->setState(4);
						monsters[i]->setHealth(monsters[i]->getHealth() - 15);
						if (monsters[i]->getHealth() < 0) {
							sound.Play(SOUND_MONSTERKILLED);
							monsters.erase(monsters.begin() + i);
							dead = true;
						}
						else sound.Play(SOUND_NPCHIT);
					}
				}
			}
			if (!dead) {		//check if monster hits player
				if (monsters[i]->getHitLeft() && player->getCdHit() == 40 && !player->isDead()) {
					player->setIsHit(true);
					player->setState(STATE_HIT_LEFT);
					player->setHealth(player->getHealth() - monsters[i]->getDamage());
					if (player->getHealth() == 0) player->setState(STATE_DEAD);
				}
				else if (monsters[i]->getHitRight() && player->getCdHit() == 40 && !player->isDead()) {
					player->setIsHit(true);
					player->setState(STATE_HIT_RIGHT);
					player->setHealth(player->getHealth() - monsters[i]->getDamage());
					if (player->getHealth() == 0) player->setState(STATE_DEAD);
				}
				if (monsters[i]->getName() == "Necromancer") {
					int numProjs = monsters[i]->getSizeProjectiles();
					for (int j = 0; j < numProjs; ++j) {
						if (monsters[i]->projectileHitsPlayerLeft(j, pos) && player->getCdHit() == 40 && !player->isDead()) {
							player->setIsHit(true);
							player->setState(STATE_HIT_LEFT);
							player->setHealth(player->getHealth() - 25);	//damage of projectile by default
							if (player->getHealth() == 0) player->setState(STATE_DEAD);
						}
						else if (monsters[i]->projectileHitsPlayerRight(j, pos) && player->getCdHit() == 40 && !player->isDead()) {
							player->setIsHit(true);
							player->setState(STATE_HIT_RIGHT);
							player->setHealth(player->getHealth() - 25);
							if (player->getHealth() == 0) player->setState(STATE_DEAD);
						}
					}
				}
			}
		}

		for (int i = 0; i < int(items.size());) {			//update items in the map
			items[i]->update(player->itsCollectable(items[i]->getName()));
			if (items[i]->getCollides()) {
				sound.Play(SOUND_GRAB);
				player->addToInv(items[i]);
				items.erase(items.begin() + i);
			}
			else ++i;
		}
		inventory->update(pos);
		craftTable->update(pos);
		projection = glm::ortho(pos.x - float(SCREEN_WIDTH) / 2, pos.x + 32 + float(SCREEN_WIDTH) / 2, pos.y + 32 + float(SCREEN_HEIGHT) / 2, pos.y - float(SCREEN_HEIGHT) / 2);
	}
}

void Scene::render()
{
	if (window != 0) {

		glm::mat4 modelview;
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		glm::vec2 pMouse = mouse->getMousePosMenu();

		if (window == MAIN) {
			glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1023.f, 767.f) };
			glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
			backgroundsMenu[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			backgroundsMenu[0]->render(textsMenu[0]);

			

			geom[0] = glm::vec2(500.f,250.f); geom[1] = glm::vec2(565.f,299.f);
			backgroundsMenu[1] = TexturedQuad::createTexturedQuad(geom,texCoords,texProgram);
			if (pMouse.x > 500 && pMouse.x < 565 && pMouse.y > 250 && pMouse.y < 299) backgroundsMenu[1]->render(textsMenu[6]);
			else backgroundsMenu[1]->render(textsMenu[5]);

			geom[0] = glm::vec2(426.f,320.f); geom[1] = glm::vec2(633.f,366.f);
			backgroundsMenu[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			if (pMouse.x > 426 && pMouse.x < 633 && pMouse.y > 320 && pMouse.y < 366) backgroundsMenu[2]->render(textsMenu[8]);
			else backgroundsMenu[2]->render(textsMenu[7]);

			geom[0] = glm::vec2(470.f,390.f); geom[1] = glm::vec2(593.f,426.f);
			backgroundsMenu[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			if (pMouse.x > 470 && pMouse.x < 593 && pMouse.y > 390 && pMouse.y < 426) backgroundsMenu[3]->render(textsMenu[10]);
			else backgroundsMenu[3]->render(textsMenu[9]);
		}
		else if (window == HOWTO) {
			glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1023.f, 767.f) };
			glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
			backgroundsMenu[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			backgroundsMenu[0]->render(textsMenu[1]);

			geom[0] = glm::vec2(400.f,650.f); geom[1] = glm::vec2(616.f,720.f);
			backgroundsMenu[1] = TexturedQuad::createTexturedQuad(geom,texCoords,texProgram);
			if (pMouse.x > 400 && pMouse.x < 616 && pMouse.y > 650 && pMouse.y < 720) {
				backgroundsMenu[1]->render(textsMenu[4]);
			}
			else {
				texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.8f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				backgroundsMenu[1]->render(textsMenu[3]);
				glDisable(GL_BLEND);
				texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			}
			
		}
		else if (window == CREDITS) {
			glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1023.f, 767.f) };
			glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
			backgroundsMenu[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			backgroundsMenu[0]->render(textsMenu[2]);

			geom[0] = glm::vec2(400.f, 650.f); geom[1] = glm::vec2(616.f, 720.f);
			backgroundsMenu[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			if (pMouse.x > 400 && pMouse.x < 616 && pMouse.y > 650 && pMouse.y < 720) {
				backgroundsMenu[1]->render(textsMenu[4]);
			}
			else {
				texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.8f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				backgroundsMenu[1]->render(textsMenu[3]);
				glDisable(GL_BLEND);
				texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		mouse->changeMouseSpriteMenu();
	}

	else if (window == PLAY) {
		if (!Init) {
			initGame();
			Init = true;
		}
		glm::mat4 modelview;
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

		for (int i = 0; i < 5; ++i) backgrounds[i]->render(texts[i]);	//render backgrounds first
		map->render();
		for (int i = 0; i < int(items.size()); ++i) items[i]->render();			//render test items
		for (int i = 0; i < int(monsters.size()) && monsters[i] != NULL; ++i) {
			monsters[i]->render();	//render monsters
			if (monsters[i]->getName() == "Necromancer") {
				monsters[i]->renderProjectiles();
			}
		}
		glm::vec2 pos = player->getPosition();

		if (player->isAttacking()) {
			string weapon = player->getWeapon();
			if (weapon == "Sword") {
				Item* wp = new Sword(0);
				wp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
				wp->setPosition(pos);
				if (player->getLastKey() == 1) wp->getSprite()->renderPlayerLeft();
				else if (player->getLastKey() == 2) wp->getSprite()->renderPlayerRight();
			}

		}

		//player blinks if hit
		if (player->getCdHit() < 40) {
			float alpha = cos(3.14159f*(float(player->getCdHit() % 5)*72.f) / 180.f);
			texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, alpha);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			player->render();
			glDisable(GL_BLEND);
			texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			player->render();
		}

		texProgram.use();
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		updateLayouts();
		texProgram.use();	//Necesary due to that Layouts use Text class, and this uses another program
		glm::mat4 pj = projection;
		inventory->render(pj);
		pj = projection;
		texProgram.use();
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		craftTable->render(pj);
		//texProgram.use();
		mouse->changeMouseSprite(pos);
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::mousePress(int button) {
	if (window == 0) {
		glm::vec2 pos = player->getPosition();
		mouse->changeMouseSprite(pos);
	}
	else {
		mouse->changeMouseSpriteMenu();
	}
}

void Scene::mouseRelease(int button) {

}

void Scene::loadBackgrounds() {
	
	texts[0].loadFromFile("images/Backgrounds/Background_0.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texts[1].loadFromFile("images/Backgrounds/Background_25.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texts[2].loadFromFile("images/Backgrounds/Background_52.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texts[3].loadFromFile("images/Backgrounds/Background_78.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texts[4].loadFromFile("images/Backgrounds/Background_66.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 geom[2] = { glm::vec2(-1000.f, -500.f), glm::vec2(6000.f, 800.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	backgrounds[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	geom[0] = glm::vec2(-1000.f, 300.f); geom[1] = glm::vec2(3096.f, 734.f);
	texCoords[1] = glm::vec2(4.f, 1.f);
	backgrounds[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	geom[0] = glm::vec2(-1000.f, 400.f); geom[1] = glm::vec2(3096.f, 734.f);
	texCoords[1] = glm::vec2(4.f, 0.656188605108055f);
	backgrounds[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	geom[0] = glm::vec2(-1000.f, 734.f); geom[1] = glm::vec2(5400.f, 3614.f);
	texCoords[1] = glm::vec2(40.f, 30.f);
	backgrounds[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	geom[0] = glm::vec2(-1000.f, 718.f); geom[1] = glm::vec2(3000.f, 734.f);
	texCoords[1] = glm::vec2(25.f, 1.f);
	backgrounds[4] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void Scene::updateBackgrounds() {
	glm::vec2 geom[2];
	glm::vec2 texCoords[2];
	glm::vec2 pos = player->getPosition();
	geom[0] = glm::vec2(-1000.f + pos.x / 1.2, -150.f + pos.y / 1.5); geom[1] = glm::vec2(3096.f + pos.x / 1.2, 618.f + pos.y / 1.5);
	texCoords[1] = glm::vec2(4.f, 1.f);
	backgrounds[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	geom[0] = glm::vec2(-1000.f + pos.x / 1.4, 50.f + pos.y / 2); geom[1] = glm::vec2(3096.f + pos.x / 1.4, 559.f + pos.y / 2);
	texCoords[1] = glm::vec2(4.f, 1.f);
	backgrounds[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void Scene::loadLayouts() {
	texts[5].loadFromFile("images/Layout/Heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texts[6].loadFromFile("images/Layout/Half_Heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texts[7].loadFromFile("images/Layout/Heart_Empty.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 pos = player->getPosition();

	//print health
	int max_H = player->getMaxHealth();
	int H = player->getHealth();
	sprintf_s(textHealthPlayer, "Life: %d/%d", H, max_H);

	//print layouts
	glm::vec2 geom[2] = { glm::vec2(pos.x + SCREEN_WIDTH / 4 - 32, pos.y - SCREEN_HEIGHT / 2 + 48), glm::vec2(pos.x + SCREEN_WIDTH / 4 + 260 - 32, pos.y + SCREEN_HEIGHT / 2 + 48 + 26) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(10.f, 1.f) };
	layouts[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void Scene::updateLayouts() {
	glm::vec2 pos = player->getPosition();
	int max_H = player->getMaxHealth();
	int H = player->getHealth();
	int numHearts = H / 20;
	int posHearts = 10 - numHearts;
	int numEmptyH = (max_H - H) / 20;
	int numHalfH;
	((max_H - H) % 20) != 0 ? numHalfH = 1 : numHalfH = 0;


	//print layouts
	//Full Hearts
	glm::vec2 geom[2] = { glm::vec2(pos.x + SCREEN_WIDTH / 4 - 32 , pos.y - SCREEN_HEIGHT / 2 + 48), glm::vec2(pos.x + SCREEN_WIDTH / 4 + (numHearts * 26) - 32, pos.y - SCREEN_HEIGHT / 2 + 48 + 26) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(numHearts), 1.f) };
	layouts[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	//Half Hearts
	geom[0] = glm::vec2(pos.x + SCREEN_WIDTH / 4 + (numHearts * 26) - 32, pos.y - SCREEN_HEIGHT / 2 + 48);
	geom[1] = glm::vec2(pos.x + SCREEN_WIDTH / 4 + ((numHearts + 1) * 26) - 32, pos.y - SCREEN_HEIGHT / 2 + 48 + 26);
	texCoords[1] = glm::vec2(float(numHalfH), 1.f);
	layouts[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	//Empty Hearts
	geom[0] = glm::vec2(pos.x + SCREEN_WIDTH / 4 - 32 + 260, pos.y - SCREEN_HEIGHT / 2 + 48);
	geom[1] = glm::vec2(pos.x + SCREEN_WIDTH / 4 - 32 + 260 - (numEmptyH * 26), pos.y - SCREEN_HEIGHT / 2 + 48 + 26);
	texCoords[1] = glm::vec2(float(numEmptyH), 1.f);
	layouts[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	layouts[0]->render(texts[5]);
	layouts[1]->render(texts[6]);
	layouts[2]->render(texts[7]);

	//print health
	sprintf_s(textHealthPlayer, "Life: %d/%d ", H, max_H);
	text.render(textHealthPlayer, glm::vec2(pos.x + SCREEN_WIDTH / 4 + 48, pos.y - SCREEN_HEIGHT / 2 + 38), 20, glm::vec4(1, 1, 1, 1), projection);

	//print dead (if so)
	if (player->getState() == STATE_DEAD) {
		sprintf_s(textD, "You were slain...");
		text.render(textD, glm::vec2(pos.x - 88, pos.y + 30), 44, glm::vec4(1, 0, 0, 0.8), projection);
	}
}

void Scene::initMonsters() {
	for (int i = 0; i < 8; ++i) {
		monsters.push_back(new Zombie);
		monsters[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		if (i < 5) monsters[i]->setPosition(glm::vec2((45 + 55*i) * map->getTileSize(), 73 * map->getTileSize()));
		else monsters[i]->setPosition(glm::vec2((45 + 55 * (i-5)) * map->getTileSize(), 105 * map->getTileSize()));
		monsters[i]->setTileMap(map);
	}

	monsters.push_back(new Necromancer);
	monsters[8]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	monsters[8]->setPosition(glm::vec2(230 * map->getTileSize(), 105 * map->getTileSize()));
	monsters[8]->setTileMap(map);
	monsters.push_back(new Necromancer);
	monsters[9]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	monsters[9]->setPosition(glm::vec2(90 * map->getTileSize(), 121 * map->getTileSize()));
	monsters[9]->setTileMap(map);
	monsters.push_back(new Necromancer);
	monsters[10]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	monsters[10]->setPosition(glm::vec2(90 * map->getTileSize(), 136 * map->getTileSize()));
	monsters[10]->setTileMap(map);
}

void Scene::setAllStates() {
	for (int i = 0; i < int(monsters.size()) && monsters[i] != NULL; ++i) {
		monsters[i]->setPosPlayer(player->getPosition());
	}
	for (int i = 0; i < int(items.size()); ++i) {
		items[i]->setPosPlayer(player->getPosition());
	}
}