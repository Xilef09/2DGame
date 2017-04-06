#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

class Scene; // include de Scene para evitar dependencia circular
#include <stdlib.h> 
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Enemy
{
public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string enemyType, string direction, Scene *scene);
	void update(int deltaTime, Player &player);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void playMusic(const string &fileName);

	glm::ivec2 posEnemy;

private:
	bool bJumping, delayAtac;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	string direccion;
	Scene *scene;

};

#endif // _ENEMY_INCLUDE

