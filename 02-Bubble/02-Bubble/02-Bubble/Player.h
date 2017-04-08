#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "ShaderProgram.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void isDead(bool dead);
	void isDeadByDoor(bool dead);
	void setLives();
	int getLives();

	glm::ivec2 posPlayer;
	Sprite *sprite;
	bool hasFireball;
	
private:
	bool bJumping, falling;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	TileMap *map;
	string direccion;
	float jumpDistance;
	int lives;
	ShaderProgram texProgram;
};


#endif // _PLAYER_INCLUDE


