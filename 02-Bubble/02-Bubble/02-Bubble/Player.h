#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

class Scene;
#include "Sprite.h"
#include "TileMap.h"
#include "ShaderProgram.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Scene* scene);
	void update(int deltaTime);
	void render();
	void renderLive();
	void renderFireball();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setLivePosition(const glm::vec2 &pos);

	void isDead(bool dead);
	void isDeadByDoor(bool dead);
	void setLives();
	int getLives();

	glm::ivec2 posPlayer;
	glm::ivec2 posFireball;
	bool hasFireball;
	Sprite *sprite, *spriteLive, *spriteFireball;

	
private:
	bool bJumping, falling;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet, spritesheet1Live, spritesheet2Live, spritesheet3Live, spritesheetGameOver, spritesheetFireball;
	TileMap *map;
	string direccion;
	float jumpDistance;
	int lives;
	ShaderProgram texProgram;
	Scene *scene;
};


#endif // _PLAYER_INCLUDE


