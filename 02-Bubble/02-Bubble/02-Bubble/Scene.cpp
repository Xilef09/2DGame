#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 5	
#define INIT_PLAYER_Y_TILES 5


Scene::Scene()
{
	map = NULL;
	player = NULL;
	enemy = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (enemy != NULL)
		delete enemy;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	mapColumns = TileMap::createTileMap("levels/level02columns.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	
	enemy=new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2((INIT_PLAYER_X_TILES-3)* map->getTileSize(), (INIT_PLAYER_Y_TILES+1) * map->getTileSize()));
	enemy->setTileMap(map);

	projection = glm::ortho(camaraX, float(SCREEN_WIDTH + camaraX), float(SCREEN_HEIGHT + camaraY),camaraY);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	enemy->update(deltaTime,player[0]);

	glm::ivec2 posicioActual = player->posPlayer;
	if (((posicioActual[0] + 32) % 320) == 0 && camaraMoguda==false){
		if ((SCREEN_WIDTH + camaraX) <= (posicioActual[0] + 32))
			camaraX = camaraX + 320;
		else
			camaraX = camaraX - 320;
		camaraMoguda = true;
	}
	else if (((posicioActual[1] + 120) % 192) == 0 && camaraMoguda == false){
		if ((SCREEN_HEIGHT + camaraY) <= (posicioActual[1] + 120)){
			camaraY = camaraY + 192;
		}
		else
			camaraY = camaraY - 192;
		camaraMoguda = true;
	}
	else camaraMoguda = false;
	projection = glm::ortho(camaraX, float(SCREEN_WIDTH + camaraX), float(SCREEN_HEIGHT + camaraY), camaraY);
}

void Scene::render()
{
	glm::mat4 modelview;

	//Mapa
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();

	//Prince
	player->render();

	enemy->render();

	//Columnes
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	mapColumns->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



