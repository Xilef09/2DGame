#ifndef _FIRE_INCLUDE
#define _FIRE_INCLUDE

#include <stdlib.h> 
#include "Sprite.h"
#include "TileMap.h"
class Fire
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string file);
	void update(int deltaTime);
	void render();

	glm::ivec2 trapPosition;

private:
	bool bJumping;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};

#endif // _FIRE_INCLUDE

