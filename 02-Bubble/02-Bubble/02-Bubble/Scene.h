#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Spike.h"
#include "SpikeDoor.h"

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

	bool initEnemies(string levelFile);
	bool initTraps(const string &file);
	void playMusic(const string &fileName);

	

private:
	void initShaders(); 

private:
	TileMap *map;
	TileMap *mapColumns;
	Player *player;
	Spike *spike;
	SpikeDoor *spikeDoor;
	Enemy *enemy;
	ShaderProgram texProgram;
	float currentTime; 
	float camaraY = 128.f, camaraX = 0.f;
	glm::mat4 projection;
	bool camaraMoguda = false;
	
	vector<Spike *> spikes;
	vector<SpikeDoor *> spikeDoors;

	vector<Enemy *> enemies;

	
};


#endif // _SCENE_INCLUDE
