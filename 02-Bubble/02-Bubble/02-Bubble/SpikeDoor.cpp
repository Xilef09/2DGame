#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "SpikeDoor.h"
#include "Game.h"

enum PlayerAnims
{
	NOTHING, ALMOST_NOTHING, HALF_VISIBLE, FULL_VISIBLE
};

void SpikeDoor::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Scene *scene)
{
	this->scene = scene;
	spritesheet.loadFromFile("images/Traps.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(NOTHING, 8);
	sprite->addKeyframe(NOTHING, glm::vec2(0.4f, 0.0f));

	sprite->setAnimationSpeed(ALMOST_NOTHING, 8);
	sprite->addKeyframe(ALMOST_NOTHING, glm::vec2(0.1f, 0.1f));

	sprite->setAnimationSpeed(HALF_VISIBLE, 8);
	sprite->addKeyframe(HALF_VISIBLE, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(FULL_VISIBLE, 8);
	sprite->addKeyframe(FULL_VISIBLE, glm::vec2(0.3f, 0.1f));

	this->player = player;

	sprite->changeAnimation(NOTHING);
	tileMapDispl = tileMapPos;
	//investigar com agafar trap position
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + trapPosition.x), float(tileMapDispl.y + trapPosition.y)));


}


void SpikeDoor::update(int deltaTime, Player* player)
{
	
	bool acabada = sprite->update(deltaTime);

	if (acabada) {
		switch (sprite->animation())
		{
		case NOTHING:
			sprite->changeAnimation(ALMOST_NOTHING);
			break;
		case ALMOST_NOTHING:
			sprite->changeAnimation(HALF_VISIBLE);
			break;
		case HALF_VISIBLE:
			sprite->changeAnimation(FULL_VISIBLE);
			break;
		case FULL_VISIBLE:
			sprite->changeAnimation(NOTHING);
			break;
		}
	}

	if ((player->posPlayer.y - tileMapDispl.y) < 2 && (player->posPlayer.x - tileMapDispl.x) == 0 && sprite->animation() != NOTHING) {
		player->setLives();
		player->isDeadByDoor(true);
	}
}

void SpikeDoor::render() {
	sprite->render();
}