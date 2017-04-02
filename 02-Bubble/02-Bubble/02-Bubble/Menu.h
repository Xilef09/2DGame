#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <stdlib.h> 
#include "Sprite.h"
#include "TileMap.h"

class Menu
{
public:
	void init(ShaderProgram &shaderProgram);
	bool update(int deltaTime);
	void render();

private:

	void print(int x, int y, int z, char *string);

	Texture spritesheet;
	Texture spritesheetinst;
	Texture spritesheetcred;
	Sprite *sprite;

	int state;
};

#endif // _MENU_INCLUDE