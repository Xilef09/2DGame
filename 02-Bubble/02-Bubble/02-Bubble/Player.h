#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void renderLive();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setLivePosition(const glm::vec2 &pos);

	void isDead(bool dead);
	void isDeadByDoor(bool dead);
	void setLives();
	int getLives();

	glm::ivec2 posPlayer;
	Sprite *sprite, *spriteLive;

	
private:
	bool bJumping, falling;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet, spritesheet1Live, spritesheet2Live, spritesheet3Live, spritesheetGameOver;
	TileMap *map;
	string direccion;
	float jumpDistance;
	int lives;
};


#endif // _PLAYER_INCLUDE


