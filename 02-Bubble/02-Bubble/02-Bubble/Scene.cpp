#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 5	
#define INIT_PLAYER_Y_TILES 3


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
	//mapTraps = TileMap::createTileMap("levels/level02traps.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	player->setTileMap(map);

	bool result = initTraps("levels/level02traps.txt");

	initEnemies("levels/level02enemies.txt");

	/*enemy=new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram,"sargent");
	enemy->setPosition(glm::vec2((INIT_PLAYER_X_TILES-2)* map->getTileSize(), (INIT_PLAYER_Y_TILES) * map->getTileSizeY()));
	enemy->setTileMap(map);*/

	projection = glm::ortho(camaraX, float(SCREEN_WIDTH + camaraX), float(SCREEN_HEIGHT + camaraY),camaraY);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	//enemy->update(deltaTime,player[0]);

	for each (Enemy *enemy in enemies) {
		enemy->update(deltaTime, player[0]);
	}

	for each (Spike *spike in spikes) {
		spike->update(deltaTime, player[0]);
	}

	for each (SpikeDoor *spikeDoor in spikeDoors) {
		spikeDoor->update(deltaTime);
	}

	//spikes->update(deltaTime);

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

	for each (Enemy *enemy in enemies){
		enemy->render();
	}

	for each (Spike *spike in spikes) {
		spike->render();
	}

	for each (SpikeDoor *spikeDoor in spikeDoors) {
		spikeDoor->render();
	}

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

bool Scene::initEnemies(string levelFile){
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream, sstream1, sstream2, sstream3, sstream4;

	int numEnemies;
	//string tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "ENEMIES") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> numEnemies;
	for (int j = 0; j < numEnemies; j++)
	{
		string enemyType;
		string direction;
		string posX;
		string posY;
		getline(fin, enemyType);
		getline(fin, direction);
		getline(fin, posX);
		getline(fin, posY);

		enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, enemyType,direction);
		enemy->setPosition(glm::vec2(stoi(posX)* map->getTileSize(), stoi(posY)* map->getTileSizeY()));
		enemy->setTileMap(map);

		enemies.push_back(enemy);

	}
}

bool Scene::initTraps(const string &file) {
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	//string tile;
	glm::ivec2 mapSize;

	fin.open(file.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	/*
	stringstream tile_size_sstream(line);
	tile_size_sstream >> tileSizeX >> tileSizeY >> blockSizeX >> blockSizeY;
	getline(fin, tilesheetFile);
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	stringstream tile_sstream(line);
	tile_sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	*/
	//map = new int[mapSize.x * mapSize.y];

	int trapsCount = 0;
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			getline(fin, line);
			string tile;
			tile = line;
			if (stoi(tile) == 1) 
			{ //spike
				spike = new Spike();
				spike->init(glm::ivec2(i*map->getTileSizeX()+16.0f, j*(map->getTileSizeY())-64.0f), texProgram);
				spikes.push_back(spike);
			}
			else if (stoi(tile) == 2)
			{
				//porta
				spikeDoor = new SpikeDoor();
				spikeDoor->init(glm::ivec2(i*map->getTileSizeX()+14.0f, j*(map->getTileSizeY())-67.0f), texProgram, player);
				spikeDoors.push_back(spikeDoor);
			}
			/*
			if (stoi(tile) == 18 || stoi(tile) == 24 || stoi(tile) == 52 ) {
			//trampa 1 pinchos

			}
			else if (stoi(tile) == 8) {
			//puerta de pichos
			}
			
			//fin.get(tile);
			if(tile.compare('0'))
			map[j*mapSize.x+i] = 0;
			else
			*/
			//map[j*mapSize.x + i] = stoi(tile);
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
	
}


