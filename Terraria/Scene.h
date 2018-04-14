#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Zombie.h"
#include "Necromancer.h"
#include "Mouse.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Dirt.h"
#include "IronOre.h"
#include "GoldOre.h"
#include "Stone.h"
#include "Wood.h"
#include "Pickaxe.h"
#include "Axe.h"
#include "Sword.h"
#include "CraftTable.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void initGame();
	void update(int deltaTime);
	void render();
	void mousePress(int button);
	void mouseRelease(int button);

private:
	void initShaders();
	void loadBackgrounds();
	void updateBackgrounds();
	void initMonsters();
	void setAllStates();
	void loadLayouts();
	void updateLayouts();

private:
	TileMap *map;
	Player *player;
	Mouse *mouse;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::ivec2 tileMapDispl;
	vector<Monster*> monsters;
	vector<Item*> items;
	Texture texts[8];
	TexturedQuad *backgrounds[5];
	TexturedQuad *layouts[3];
	Texture textsMenu[11];
	TexturedQuad *backgroundsMenu[4];
	Text text;
	char textD[20];
	char textHealthPlayer[20];
	Sound sound;
	Inventory *inventory;
	CraftTable *craftTable;
	int window;
	bool Init;
};


#endif // _SCENE_INCLUDE

