#ifndef _SPIKEDOOR_INCLUDE
#define _SPIKEDOOR_INCLUDE

#include "Sprite.h"
#include "Player.h"

class SpikeDoor
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *player);
	void update(int deltaTime);
	void render();


	glm::ivec2 trapPosition;
	int trapId;

private:
	bool bJumping;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;

};

#endif // _SPIKEDOOR_INCLUDE
