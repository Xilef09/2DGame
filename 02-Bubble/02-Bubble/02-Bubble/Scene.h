#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Spikes.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders(); 

private:
	TileMap *map;
	TileMap *mapColumns;
	TileMap *mapTraps;
	Player *player;
	Enemy *enemy;
	ShaderProgram texProgram;
	float currentTime; 
	float camaraY = 128.f, camaraX = 0.f;
	glm::mat4 projection;
	bool camaraMoguda = false;

	Spikes *traps = new Spikes[18];
};


#endif // _SCENE_INCLUDE

