#ifndef _SPIKEDOOR_INCLUDE
#define _SPIKEDOOR_INCLUDE

class Scene;
#include "Sprite.h"
#include "Player.h"

class SpikeDoor
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Scene *scene);
	void update(int deltaTime, Player *player);
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

	Scene *scene;

};

#endif // _SPIKEDOOR_INCLUDE
