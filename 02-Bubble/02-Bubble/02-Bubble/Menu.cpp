#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Menu.h"
#include "Game.h"

enum FireAnims
{
	MENU
};

void Menu::init(ShaderProgram &shaderProgram){

	state = 0;

	//Menu background
	spritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetinst.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetcred.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 192), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MENU , 8);
	sprite->addKeyframe(MENU, glm::vec2(0.f, 0.0f));

	sprite->changeAnimation(MENU);
	sprite->setPosition(glm::vec2(0.f, 0.f));
}

bool Menu::update(int deltaTime)
{
	switch (state)
	{
	case 0:
		if (Game::instance().getSpecialKey(GLUT_KEY_F2)){
			sprite->changeSpitesheet(&spritesheetinst);
			state = 1;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_F3)){
			sprite->changeSpitesheet(&spritesheetcred);
			state = 2;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_F1)){
			state = 0;
			return true;
		}
		break;
	case 1:
		if (Game::instance().getSpecialKey(GLUT_KEY_F9)){
			sprite->changeSpitesheet(&spritesheet);
			state = 0;
		}
		break;
	case 2:
		if (Game::instance().getSpecialKey(GLUT_KEY_F9)){
			sprite->changeSpitesheet(&spritesheet);
			state = 0;
		}
		break;
	}
	return false;
}

void Menu::render(){
	sprite->render();
}