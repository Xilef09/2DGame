#include "Sprite.h"
#include "Player.h"

class Spikes
{
public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *player);
	void update(int deltaTime);
	void render();

	
	glm::ivec2 trapPosition;
	int trapId;

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;

};


