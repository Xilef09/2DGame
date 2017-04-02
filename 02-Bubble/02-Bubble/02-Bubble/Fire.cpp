#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


enum FireAnims
{
	FIRE
};

void Fire::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string file)
{
	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(FIRE, 8);
	sprite->addKeyframe(FIRE, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.3f, 0.0f));

	sprite->changeAnimation(FIRE);
	tileMapDispl = tileMapPos;
	//investigar com agafar trap position
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + trapPosition.x), float(tileMapDispl.y + trapPosition.y)));
}

void Fire::update(int deltaTime)
{
	bool acabada = sprite->update(deltaTime);
	if (acabada) sprite->changeAnimation(FIRE);
}

void Fire::render() {
	sprite->render();
}