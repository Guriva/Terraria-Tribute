#pragma once
#ifndef _ZOMBIE_INCLUDE
#define _ZOMBIE_INCLUDE

#include "Monster.h"

class Zombie : public Monster
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	glm::vec2 getSize();
};
#endif // _ZOMBIE_INCLUDE