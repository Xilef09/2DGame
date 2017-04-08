
#ifndef _FIREBALL_INCLUDE
#define _FIREBALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Fireball
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
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

#endif // _FIREBALL_INCLUDE

