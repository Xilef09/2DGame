#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 2
#define JUMP_HEIGHT 65
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT, JUMP_RUN_RIGHT, JUMP_RUN_LEFT,
	JUMP_STAND_LEFT, CHANGE_DIRECTION_LEFT, CHANGE_DIRECTION_RIGHT, JUMP_STAND_RIGHT, START_MOVING_RIGHT, 
	START_MOVING_LEFT, CLIMB_RIGHT, CLIMB_LEFT, ATTACK_RIGHT, ATTACK_LEFT, SPIKE_DEATH_LEFT, SPIKE_DEATH_RIGHT,
	SPIKE_DOOR_DEATH_RIGHT, SPIKE_DOOR_DEATH_LEFT, DEATH_LEFT, DEATH_RIGHT
};

enum LiveAnims{
	LIVES
};

enum FireballAnims {
	NOTHING, MOVING_RIGHT, MOVING_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Scene *scene)
{
	this->scene = scene;

	hasFireball = false;
	//this->fireball = fireball;
	//this->fireball->init(tileMapPos, shaderProgram);
	lives = 3;
	direccion = "";
	bJumping = false;

	//Barra de vida
	spritesheet1Live.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet1Live.loadFromFile("images/livebar1.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spritesheet2Live.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet2Live.loadFromFile("images/livebar2.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spritesheet3Live.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet3Live.loadFromFile("images/livebar3.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spritesheetGameOver.setWrapS(GL_MIRRORED_REPEAT);
	spritesheetGameOver.loadFromFile("images/livebarover.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spriteLive = Sprite::createSprite(glm::ivec2(320, 10), glm::vec2(1, 1), &spritesheet3Live, &shaderProgram);
	spriteLive->setNumberAnimations(1);

	spriteLive->setAnimationSpeed(LIVES, 8);
	spriteLive->addKeyframe(LIVES, glm::vec2(0.f, 0.f));

	spriteLive->changeAnimation(LIVES);
	spriteLive->setPosition(glm::vec2(0.f, 320.f));

	//Fireball
	spritesheetFireball.setWrapS(GL_MIRRORED_REPEAT);
	spritesheetFireball.loadFromFile("images/fireballSpriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteFireball = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheetFireball, &shaderProgram);
	spriteFireball->setNumberAnimations(3);

	spriteFireball->setAnimationSpeed(NOTHING, 8);
	spriteFireball->addKeyframe(NOTHING, glm::vec2(0.9f, 0.0f));

	spriteFireball->setAnimationSpeed(MOVING_RIGHT, 8);
	spriteFireball->addKeyframe(MOVING_RIGHT, glm::vec2(0.1f, 0.0f));
	spriteFireball->addKeyframe(MOVING_RIGHT, glm::vec2(0.2f, 0.0f));
	spriteFireball->addKeyframe(MOVING_RIGHT, glm::vec2(0.3f, 0.0f));
	spriteFireball->addKeyframe(MOVING_RIGHT, glm::vec2(0.4f, 0.0f));
	spriteFireball->addKeyframe(MOVING_RIGHT, glm::vec2(0.5f, 0.0f));
	spriteFireball->addKeyframe(MOVING_RIGHT, glm::vec2(0.6f, 0.0f));

	spriteFireball->setAnimationSpeed(MOVING_LEFT, 8);
	spriteFireball->addKeyframe(MOVING_LEFT, glm::vec2(-0.1f, 0.0f));
	spriteFireball->addKeyframe(MOVING_LEFT, glm::vec2(-0.2f, 0.0f));
	spriteFireball->addKeyframe(MOVING_LEFT, glm::vec2(-0.3f, 0.0f));
	spriteFireball->addKeyframe(MOVING_LEFT, glm::vec2(-0.4f, 0.0f));
	spriteFireball->addKeyframe(MOVING_LEFT, glm::vec2(-0.5f, 0.0f));
	spriteFireball->addKeyframe(MOVING_LEFT, glm::vec2(-0.6f, 0.0f));

	spriteFireball->changeAnimation(NOTHING);
	spriteFireball->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//Player sprites
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/princeSpriteSheet2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(24);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.f));
		
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(START_MOVING_LEFT, 8);
	sprite->addKeyframe(START_MOVING_LEFT, glm::vec2(-0.1f, 0.1f));
	sprite->addKeyframe(START_MOVING_LEFT, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(START_MOVING_LEFT, glm::vec2(-0.3f, 0.1f));
	sprite->addKeyframe(START_MOVING_LEFT, glm::vec2(-0.4f, 0.1f));

	sprite->setAnimationSpeed(SPIKE_DEATH_LEFT, 8);
	sprite->addKeyframe(SPIKE_DEATH_LEFT, glm::vec2(-0.0f, 0.8f));
	sprite->addKeyframe(SPIKE_DEATH_LEFT, glm::vec2(-0.1f, 0.8f));
	sprite->addKeyframe(SPIKE_DEATH_LEFT, glm::vec2(-0.2f, 0.8f));

	sprite->setAnimationSpeed(SPIKE_DEATH_RIGHT, 8);
	sprite->addKeyframe(SPIKE_DEATH_RIGHT, glm::vec2(0.0f, 0.8f));
	sprite->addKeyframe(SPIKE_DEATH_RIGHT, glm::vec2(0.1f, 0.8f));
	sprite->addKeyframe(SPIKE_DEATH_RIGHT, glm::vec2(0.2f, 0.8f));

	sprite->setAnimationSpeed(DEATH_LEFT, 8);
	sprite->addKeyframe(DEATH_LEFT, glm::vec2(-0.4f, 0.8f));
	sprite->addKeyframe(DEATH_LEFT, glm::vec2(-0.5f, 0.8f));
	sprite->addKeyframe(DEATH_LEFT, glm::vec2(-0.6f, 0.8f));

	sprite->setAnimationSpeed(DEATH_RIGHT, 8);
	sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.4f, 0.8f));
	sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.5f, 0.8f));
	sprite->addKeyframe(DEATH_RIGHT, glm::vec2(0.6f, 0.8f));

	sprite->setAnimationSpeed(SPIKE_DOOR_DEATH_RIGHT, 8);
	sprite->addKeyframe(SPIKE_DOOR_DEATH_RIGHT, glm::vec2(0.7f, 0.8f));
	sprite->addKeyframe(SPIKE_DOOR_DEATH_RIGHT, glm::vec2(0.8f, 0.8f));
	sprite->addKeyframe(SPIKE_DOOR_DEATH_RIGHT, glm::vec2(0.9f, 0.8f));

	sprite->setAnimationSpeed(SPIKE_DOOR_DEATH_LEFT, 8);
	sprite->addKeyframe(SPIKE_DOOR_DEATH_LEFT, glm::vec2(-0.7f, 0.8f));
	sprite->addKeyframe(SPIKE_DOOR_DEATH_LEFT, glm::vec2(-0.8f, 0.8f));
	sprite->addKeyframe(SPIKE_DOOR_DEATH_LEFT, glm::vec2(-0.9f, 0.8f));
	
	sprite->setAnimationSpeed(START_MOVING_RIGHT, 8);
	sprite->addKeyframe(START_MOVING_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(START_MOVING_RIGHT, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(START_MOVING_RIGHT, glm::vec2(0.3f, 0.1f));
	sprite->addKeyframe(START_MOVING_RIGHT, glm::vec2(0.4f, 0.1f));
				
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.7f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.8f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.9f, 0.1f));
		
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
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

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.1f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.2f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.3f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.4f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.5f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.6f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.7f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.8f, 0.2f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(-0.9f, 0.2f));

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


	sprite->setAnimationSpeed(JUMP_RUN_LEFT, 8);
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.1f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.2f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.3f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.4f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.5f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.6f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.7f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.8f, 0.3f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.9f, 0.3f));

	sprite->setAnimationSpeed(JUMP_STAND_RIGHT, 10);
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.1f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.3f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.8f, 0.5f));
		

	sprite->setAnimationSpeed(JUMP_STAND_LEFT, 10);
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.1f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.2f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.3f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.4f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.5f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.6f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.7f, 0.5f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.8f, 0.5f));
		

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

	sprite->setAnimationSpeed(CLIMB_RIGHT, 8);
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.1f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.2f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.3f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.4f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.5f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.7f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.8f, 0.6f));
	sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.9f, 0.6f));

	sprite->setAnimationSpeed(CLIMB_LEFT, 8);
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.1f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.2f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.3f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.4f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.5f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.6f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.7f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.8f, 0.6f));
	sprite->addKeyframe(CLIMB_LEFT, glm::vec2(-0.9f, 0.6f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.1f, 0.7f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.2f, 0.7f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.3f, 0.7f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.4f, 0.7f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.5f, 0.7f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.6f, 0.7f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.7f, 0.7f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 8);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.1f, 0.7f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.2f, 0.7f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.3f, 0.7f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.4f, 0.7f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.5f, 0.7f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.6f, 0.7f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.7f, 0.7f));
		

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));	
}
 
void Player::update(int deltaTime)
{
	bool fireballAcabada = spriteFireball->update(deltaTime);
	if (fireballAcabada) {
		if (hasFireball && fireballDirection == "RIGHT")
			spriteFireball->changeAnimation(MOVING_RIGHT);
		else if (hasFireball && fireballDirection == "LEFT")
			spriteFireball->changeAnimation(MOVING_LEFT);
		else 
			spriteFireball->changeAnimation(NOTHING);
	}
	bool acabada = sprite->update(deltaTime);
	if (acabada) {
		switch (sprite->animation())
		{
		case STAND_LEFT:
			direccion = "LEFT";
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(START_MOVING_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_STAND_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(STAND_RIGHT);
			else if (Game::instance().getSpecialKey(112)) sprite->changeAnimation(ATTACK_LEFT); // 112 shift
			else if (Game::instance().getSpecialKey(GLUT_KEY_F8)) {
				hasFireball = true;
				posFireball.x = posPlayer.x - 32.0f;
				posFireball.y = posPlayer.y + 10.0f;
				fireballDirection = "LEFT";
			}
			else sprite->changeAnimation(STAND_LEFT);
			scene->stopSound();
			break;
		case STAND_RIGHT:
			direccion = "RIGHT";
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(START_MOVING_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_STAND_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(STAND_LEFT);
			else if (Game::instance().getSpecialKey(112)) sprite->changeAnimation(ATTACK_RIGHT); // 112 shift
			else if (Game::instance().getSpecialKey(GLUT_KEY_F8)) {
				hasFireball = true;
				posFireball.x = posPlayer.x + 32.0f;
				posFireball.y = posPlayer.y + 10.0f;
				fireballDirection = "RIGHT";
			}
			else sprite->changeAnimation(STAND_RIGHT);
			scene->stopSound();
			break;
		case START_MOVING_RIGHT:
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(START_MOVING_RIGHT);
			else sprite->changeAnimation(MOVE_RIGHT);
			scene->playSound("music/walking.ogg");
			break;
		case START_MOVING_LEFT:
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(START_MOVING_LEFT);
			else sprite->changeAnimation(MOVE_LEFT);
			scene->playSound("music/walking.ogg");
			break;
		case MOVE_LEFT:
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(CHANGE_DIRECTION_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_LEFT);
			else sprite->changeAnimation(STAND_LEFT);
			scene->playSound("music/walking.ogg");
			break;
		case MOVE_RIGHT:
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(CHANGE_DIRECTION_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_RIGHT);
			else sprite->changeAnimation(STAND_RIGHT);
			scene->playSound("music/walking.ogg");
			break;
		case JUMP_RIGHT:
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(MOVE_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(STAND_LEFT);
			else sprite->changeAnimation(STAND_RIGHT);
			break;
		case JUMP_LEFT:
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(STAND_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(MOVE_LEFT);
			else sprite->changeAnimation(STAND_LEFT);
			break;
		case JUMP_RUN_RIGHT:
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(CHANGE_DIRECTION_LEFT);
			else sprite->changeAnimation(MOVE_RIGHT);
			break;
		case JUMP_RUN_LEFT:
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(CHANGE_DIRECTION_RIGHT);
			else sprite->changeAnimation(MOVE_LEFT);
			break;
		case JUMP_STAND_RIGHT:
			//mirar el caso en el que se puede subir ---> CLIMB_RIGHT
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(START_MOVING_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(STAND_LEFT);
			else sprite->changeAnimation(STAND_RIGHT);
			break;
		case JUMP_STAND_LEFT:
			//mirar el caso en el que se puede subir ---> CLIMB_LEFT
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(STAND_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(START_MOVING_LEFT);
			else sprite->changeAnimation(STAND_LEFT);
			break;
		case CHANGE_DIRECTION_LEFT:
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_LEFT);
			else sprite->changeAnimation(MOVE_LEFT);
			break;
		case CHANGE_DIRECTION_RIGHT:
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_RIGHT);
			else sprite->changeAnimation(MOVE_RIGHT);
			break;
		case CLIMB_RIGHT:
			sprite->changeAnimation(STAND_RIGHT);
			// faltaria mover al personaje al lugar adecuado
			break;
		case CLIMB_LEFT:
			sprite->changeAnimation(STAND_LEFT);
			// faltaria mover al personaje al lugar adecuado
			break;
		case ATTACK_RIGHT:
			sprite->changeAnimation(STAND_RIGHT);
			
			break;
		case ATTACK_LEFT:
			sprite->changeAnimation(STAND_LEFT);
			
			break;
		case SPIKE_DEATH_RIGHT:
			//restart level
			break;
		case SPIKE_DEATH_LEFT:
			//restart leve:
			break;
		case SPIKE_DOOR_DEATH_RIGHT:
			//restart level
			break;
		case DEATH_RIGHT:
			//restart level
			break;
		case DEATH_LEFT:
			//restart level
			break;
		}
	}
	/*
	if (sprite->animation() == JUMP_LEFT || sprite->animation() == START_MOVING_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_RUN_LEFT ||
		sprite->animation() == CHANGE_DIRECTION_RIGHT) {
		if (!map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) posPlayer.x -= 1;
		else sprite->changeAnimation(STAND_LEFT);
	}
	else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == START_MOVING_RIGHT || sprite->animation() == JUMP_RUN_RIGHT || sprite->animation() == MOVE_RIGHT ||
		sprite->animation() == CHANGE_DIRECTION_LEFT) {
		if (!map->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) posPlayer.x += 1;
		else sprite->changeAnimation(STAND_RIGHT);
	}
	*/

	if (sprite->animation() == JUMP_LEFT && !map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))){
		bJumping = false;
		posPlayer.x -= 1;
	}
	else if (sprite->animation() == START_MOVING_LEFT && !map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) posPlayer.x -= 1;
	else if (sprite->animation() == MOVE_LEFT && !map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) posPlayer.x -= 1;
	else if (sprite->animation() == JUMP_RUN_LEFT && !map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) posPlayer.x -= 1;
	else if (sprite->animation() == CHANGE_DIRECTION_RIGHT && !map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) posPlayer.x += 0.5;
	else if (sprite->animation() == JUMP_LEFT || sprite->animation() == START_MOVING_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_RUN_LEFT ||
		sprite->animation() == CHANGE_DIRECTION_RIGHT) {
		sprite->changeAnimation(STAND_LEFT);
		posPlayer.x += 2; // sin esta linea detecta colision siempre y no te puedes mover
	}
	else if (sprite->animation() == JUMP_RIGHT && !map->collisionMoveRight(posPlayer, glm::ivec2(32, 64))){
		bJumping = false;
		posPlayer.x += 1;
	}
	else if (sprite->animation() == START_MOVING_RIGHT && !map->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) posPlayer.x += 1;
	else if (sprite->animation() == JUMP_RUN_RIGHT && !map->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) posPlayer.x += 1;
	else if (sprite->animation() == MOVE_RIGHT && !map->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) posPlayer.x += 1;
	else if (sprite->animation() == CHANGE_DIRECTION_LEFT && !map->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) posPlayer.x -= 0.5;
	else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == START_MOVING_RIGHT || sprite->animation() == JUMP_RUN_RIGHT || sprite->animation() == MOVE_RIGHT ||
		sprite->animation() == CHANGE_DIRECTION_LEFT) {
		sprite->changeAnimation(STAND_RIGHT);
		posPlayer.x -= 2; // igual que en el caso anterior
	}
	
	/*FIREBALL collisions*/
	if (spriteFireball->animation() == MOVING_RIGHT && map->collisionMoveRight(posFireball, glm::ivec2(50, 64))) {
		hasFireball = false;
		fireballDirection = "";
	}
	else if (spriteFireball->animation() == MOVING_LEFT && map->collisionMoveLeft(posFireball, glm::ivec2(50, 64))) {
		hasFireball = false;
		fireballDirection = "";
	}


	if(bJumping && !map->collisionMoveUp(posPlayer,glm::ivec2(32,63)))
	{	
		
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y);
		}	
		
	}
	else
	{
		bJumping = false;
		jumpHeight = 65.0f;
		if (sprite->animation() != JUMP_RUN_LEFT
			&& sprite->animation() != JUMP_RUN_RIGHT
			&& sprite->animation() != JUMP_LEFT
			&& sprite->animation() != JUMP_RIGHT
			&& sprite->animation() != JUMP_STAND_RIGHT
			&& sprite->animation() != JUMP_STAND_LEFT) posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y)) //A partir d'aquests numeros es defineix el padding de la tile a terra
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) 
				&& (sprite->animation() == STAND_RIGHT || sprite->animation() == STAND_LEFT))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			
		}
	}
	
	if (lives == 2) spriteLive->changeSpitesheet(&spritesheet2Live);
	else if (lives == 1) spriteLive->changeSpitesheet(&spritesheet1Live);
	else if (lives == 0) spriteLive->changeSpitesheet(&spritesheetGameOver);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if (hasFireball) {
		if (fireballDirection == "LEFT") posFireball.x -= 1.0f;
		else posFireball.x += 1.0f;
		spriteFireball->setPosition(glm::vec2(float(tileMapDispl.x + posFireball.x), float(tileMapDispl.y + posFireball.y)));
	}
	//28 i 29 tiles puerta
	if (map->collisionWithFinalDoor(posPlayer, glm::ivec2(32, 64))) {
		//scene->nextLevel2 = true;
		scene->playMusic("music/intro_theme.ogg"); // change for win sound
	}
}

void Player::render()
{
	sprite->render();
}

void Player::renderLive(){
	spriteLive->render();
}

void Player::renderFireball() {
	spriteFireball->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	spriteFireball->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::isDead(bool isDead) {
	if (isDead) {		
		sprite->changeAnimation(SPIKE_DEATH_RIGHT);
		//sprite->changeAnimation(SPIKE_DOOR_DEATH_RIGHT);
	}
}

void Player::isDeadByDoor(bool isDead) {
	if (isDead) {
		//sprite->changeAnimation(SPIKE_DEATH_RIGHT);
		sprite->changeAnimation(SPIKE_DOOR_DEATH_RIGHT);
	}
}

void Player::setLives(){
	lives = lives -1;
}

int Player::getLives(){
	return lives;
}

void Player::setLivePosition(const glm::vec2 &pos){
	spriteLive->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

