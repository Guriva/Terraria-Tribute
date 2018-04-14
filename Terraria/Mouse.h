#ifndef _MOUSE_INCLUDE
#define _MOUSE_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"

// This class is for changing the sprite used for the mouse


class Mouse
{

public:

	void changeMouseSprite(const glm::vec2 pos);
	void changeMouseSpriteMenu();
	void createMouse(ShaderProgram &shaderProgram, const glm::vec2 &pos);
	glm::vec2 getMousePos(glm::vec2 pos);
	glm::vec2 getMousePosMenu();
	void setMousePos(int x, int y);

private:
	Texture spritesheetMouse;
	Sprite *spriteMouse;
	glm::ivec2 posMouse;
	ShaderProgram program;


};


#endif // _MOUSE_INCLUDE

