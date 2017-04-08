#include "Fireball.h"



enum PlayerAnims
{
	NOTHING
};

void Fireball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	spritesheet.loadFromFile("images/fireballSpriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NOTHING, 8);
	sprite->addKeyframe(NOTHING, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(NOTHING, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(NOTHING, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(NOTHING, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(NOTHING, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(NOTHING, glm::vec2(0.6f, 0.0f));

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + trapPosition.x), float(tileMapDispl.y + trapPosition.y)));
}


void Fireball::update(int deltaTime) {
	bool acabada = sprite->update(deltaTime);
	if (acabada) sprite->changeAnimation(NOTHING);
}


void Fireball::render() {
	sprite->render();
}
