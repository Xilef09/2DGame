#ifndef _SPIKE_INCLUDE
#define _SPIKE_INCLUDE

#include "Sprite.h"
#include "Player.h"

class Spike
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, Player *player);
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

#endif // _SPIKE_INCLUDE