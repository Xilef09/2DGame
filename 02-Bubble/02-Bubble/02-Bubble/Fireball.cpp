#include "Fireball.h"
#include "Player.h"


enum PlayerAnims
{
	NOTHING
};

void Fireball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	spritesheet.loadFromFile("images/fireballSpriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NOTHING, 8);
	sprite->addKeyframe(NOTHING, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(NOTHING, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(NOTHING, glm::vec2(0.3f, 0.0f));

	sprite->changeAnimation(NOTHING);
	tileMapDispl = tileMapPos;
}





void Fireball::render() {
	sprite->render();
}


