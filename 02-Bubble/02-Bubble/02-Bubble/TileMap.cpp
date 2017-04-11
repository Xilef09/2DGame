#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "TileMap.h"
#include "Scene.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	//string tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
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
	
	map = new int[mapSize.x * mapSize.y];

	int trapsCount = 0;
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			getline(fin,line);
			string tile;
			tile = line;
			/*
			if (stoi(tile) == 18 || stoi(tile) == 24 || stoi(tile) == 52 ) {
				//trampa 1 pinchos
				
			}
			else if (stoi(tile) == 8) {
				//puerta de pichos
			}
			*/
			//fin.get(tile);
			/*if(tile.compare('0'))
				map[j*mapSize.x+i] = 0;
			else*/
				map[j*mapSize.x+i] = stoi(tile);
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSizeX, minCoords.y-63 + j * tileSizeY);
				texCoordTile[0] = glm::vec2(float((tile-1)%10) / tilesheetSize.x, float((tile-1)/10) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSizeX); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSizeX); vertices.push_back(posTile.y + blockSizeY);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSizeX); vertices.push_back(posTile.y + blockSizeY);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSizeY);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.
bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSizeX;
	x1 = (pos.x + size.x - 1) / tileSizeX;
	y = (pos.y +1 + size.y - 1) / tileSizeY;


	if (map[(y-1)*mapSize.x + x0] != 1 && map[(y-1)*mapSize.x + x0] != 2 && map[(y-1)*mapSize.x + x0] != 12 
		&& map[(y-1)*mapSize.x + x0] != 13 && map[(y-1)*mapSize.x + x0] != 14 && map[(y-1)*mapSize.x + x0] != 17
		&& map[(y-1)*mapSize.x + x0] != 19 && map[(y-1)*mapSize.x + x0] != 20 && map[(y-1)*mapSize.x + x0] != 27
		&& map[(y-1)*mapSize.x + x0] != 31 && map[(y-1)*mapSize.x + x0] != 32 && map[(y-1)*mapSize.x + x0] != 33
		&& map[(y-1)*mapSize.x + x0] != 34 && map[(y-1)*mapSize.x + x0] != 40 && map[(y-1)*mapSize.x + x0] != 41)
	{
		return true;
	}
	return false;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = (pos.x / tileSizeX) - 1;
	y0 = pos.y / tileSizeY;
	//y1 = (pos.y + size.y - 1) / tileSizeY;
	//for (int y = y0; y <= y1; y++){
	if ((map[(y0 + 1)*mapSize.x + x + 1] == 3 && ((y0 + 1)*mapSize.x + x + 1)%98 == 0)
		|| (map[(y0 + 1)*mapSize.x + x + 1] == 4 && ((y0 + 1)*mapSize.x + x + 1) % 98 == 0)
		|| map[(y0 + 1)*mapSize.x + x + 2] == 3
		|| map[(y0 + 1)*mapSize.x + x + 2] == 4 
		|| map[(y0 + 1)*mapSize.x + x + 2] == 2 
		|| map[(y0 + 1)*mapSize.x + x + 2] == NULL)
		return true;
	//}	
	return false;
	/*int x, y0, y1;

	x = ((pos.x + size.x - 1) / tileSizeX) + 1;
	y0 = pos.y / tileSizeY;
	//y1 = (pos.y + size.y - 1) / tileSizeY;
	//for (int y = y0; y <= y1; y++){
	if (map[(y0 + 1)*mapSize.x + x] == 3 || map[(y0 + 1)*mapSize.x + x] == 23 || map[(y0 + 1)*mapSize.x + x] == 4) return true;
	//}	
	return false;
	*/
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	
	int x, y0, y1;
	
	x = ((pos.x + size.x - 1) / tileSizeX)+1;
	y0 = pos.y / tileSizeY;
	//y1 = (pos.y + size.y - 1) / tileSizeY;
	//for(int y=y0; y<=y1; y++)
	//{
	if (map[(y0 + 1)*mapSize.x + x - 1] == 3 || map[(y0 + 1)*mapSize.x + x - 1] == 2 || map[(y0 + 1)*mapSize.x + x - 1] == 4)
			return true;
	//}
	
	return false;
	
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	/*
	int x0, x1, y;
	
	x0 = pos.x / tileSizeX;
	//x1 = (pos.x + size.x - 1) / tileSizeX;
	y = (pos.y + size.y - 1) / tileSizeY;
	//for(int x=x0; x<=x1; x++)
	//{
	if (map[y*mapSize.x + x0] == 1 || map[(y)*mapSize.x + x0] == 2 || map[(y)*mapSize.x + x0] == 3
		|| map[(y)*mapSize.x + x0] == 12 || map[(y)*mapSize.x + x0] == 14 || map[(y)*mapSize.x + x0] == 17
		|| map[(y)*mapSize.x + x0] == 31 || map[(y)*mapSize.x + x0] == 32 || map[(y)*mapSize.x + x0] == 33
		|| map[(y)*mapSize.x + x0] == 34 || map[(y)*mapSize.x + x0] == 39 || map[(y)*mapSize.x + x0] == 40
		|| map[(y)*mapSize.x + x0] == 41 || map[(y)*mapSize.x + x0] == 20)
		{
			return false;
		}
	else {
		if (*posY - tileSizeY * y + size.y <= 4)
		{
			*posY = tileSizeY * y - size.y;
			return true;
		}
	}
	//}
	//return true;
	*/
	int x0, x1, y;
	
	x0 = pos.x / tileSizeX;
	x1 = (pos.x + size.x/2 - 1) / tileSizeX;
	y = (pos.y + size.y - 1) / tileSizeY;
	for(int x=x0; x<=x1; x++)
	{
		if ((map[(y)*mapSize.x + x + 1] == 13
			|| map[(y)*mapSize.x + x + 1] == 12
			|| map[(y)*mapSize.x + x + 1] == 19
			|| map[(y)*mapSize.x + x + 1] == 20
			|| map[(y)*mapSize.x + x + 1] == 17
			|| map[(y)*mapSize.x + x + 1] == 27
			|| map[(y)*mapSize.x + x + 1] == 40)
			&& pos.x % tileSizeX > 6){
			return false;
		}
		else if (map[(y)*mapSize.x + x + 1] != 1 
			&& map[(y)*mapSize.x + x + 1] != 31 
			&& map[(y)*mapSize.x + x + 1] != 32 
			&& map[(y)*mapSize.x + x + 1] != 33 
			&& map[(y)*mapSize.x + x + 1] != 34 
			&& map[(y)*mapSize.x + x + 1] != 2)
		{
			if(*posY - tileSizeY * y + size.y <= 4)
			{
				*posY = tileSizeY * y - size.y;
				return true;
			}
		}
	}
	return false;
}


bool TileMap::collisionWithSpikeDoor(const glm::ivec2 &pos, const glm::ivec2 &size) const
{

	int x, y0, y1;
	x = ((pos.x + size.x - 1) / 32.0f) + 1;
	y0 = pos.y / 64.0f;
	//y1 = (pos.y + size.y - 1) / tileSizeY;
	//for(int y=y0; y<=y1; y++)
	//{
	if (map[(y0)*mapSize.x + x - 1] == 8)
		return true;
	//}

	return false;

}

bool TileMap::collisionWithFinalDoor(const glm::ivec2 &pos, const glm::ivec2 &size) const
{

	int x0, x1, y;

	x0 = pos.x / tileSizeX;
	x1 = (pos.x + size.x / 2 - 1) / tileSizeX;
	y = (pos.y+1 + size.y - 1) / tileSizeY;
	for (int x = x0; x <= x1; x++)
	{
		if ((map[(y)*mapSize.x + x + 1] == 28 || map[(y)*mapSize.x + x + 1] == 29)) {
			return true;
		}
	}
	return false;

}
































