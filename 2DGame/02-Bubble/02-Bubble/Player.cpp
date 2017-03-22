#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_RUN_RIGHT, JUMP,
	CHANGE_DIRECTION_LEFT, CHANGE_DIRECTION_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	
	direccion = "";
	bJumping = false;
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/princeSpriteSheet2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
				
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.3f, 0.1f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.1f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.1f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.1f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.7f, 0.1f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.8f, 0.1f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.9f, 0.1f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.7f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.9f, 0.1f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.1f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.2f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.4f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.5f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.6f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.7f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.8f, 0.2f));
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.9f, 0.2f));

		sprite->setAnimationSpeed(JUMP_RUN_RIGHT, 8);
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.0f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.1f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.2f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.3f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.4f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.5f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.6f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.7f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.8f, 0.3f));
		sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.9f, 0.3f));

		sprite->setAnimationSpeed(JUMP, 8);
		sprite->addKeyframe(JUMP, glm::vec2(0.1f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.2f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.3f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.4f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.5f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.6f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.7f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.8f, 0.5f));
		sprite->addKeyframe(JUMP, glm::vec2(0.9f, 0.5f));

		sprite->setAnimationSpeed(CHANGE_DIRECTION_LEFT, 8);
		sprite->addKeyframe(CHANGE_DIRECTION_LEFT, glm::vec2(0.4f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_LEFT, glm::vec2(0.5f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_LEFT, glm::vec2(0.6f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_LEFT, glm::vec2(0.7f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_LEFT, glm::vec2(0.8f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_LEFT, glm::vec2(0.9f, 0.4f));

		sprite->setAnimationSpeed(CHANGE_DIRECTION_RIGHT, 8);
		sprite->addKeyframe(CHANGE_DIRECTION_RIGHT, glm::vec2(-0.4f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_RIGHT, glm::vec2(-0.5f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_RIGHT, glm::vec2(-0.6f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_RIGHT, glm::vec2(-0.7f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_RIGHT, glm::vec2(-0.8f, 0.4f));
		sprite->addKeyframe(CHANGE_DIRECTION_RIGHT, glm::vec2(-0.9f, 0.4f));
	
		sprite->changeAnimation(0);
		tileMapDispl = tileMapPos;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));	
}

void Player::update(int deltaTime)
{
	
	bool acabada = sprite->update(deltaTime);

	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (acabada) {
			if (sprite->animation() == STAND_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			else if (sprite->animation() == STAND_RIGHT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_LEFT);
		}
		else{
			if (sprite->animation() == MOVE_RIGHT) posPlayer.x += 1;
			else posPlayer.x -= 1;
		}
		
		/*
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
		*/
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (acabada) {
			if (sprite->animation() == STAND_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			else if (sprite->animation() == STAND_LEFT)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_RIGHT);
		}
		else{
			if (sprite->animation() == MOVE_LEFT) posPlayer.x -= 1;
			else posPlayer.x += 1;
		}
		
		/*
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
		*/
	}
	else
	{
		if (acabada) {
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
		
	}	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y)) //A partir d'aquests numeros es defineix el padding de la tile a terra

		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




