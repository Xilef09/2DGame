#include "Sprite.h"
#include "Player.h"

class Spikes
{
public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *player);
	void update(int deltaTime);
	void render();

	

private:
	glm::ivec2 tileMapDispl;
	glm::ivec2 trapPosition;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;

};


