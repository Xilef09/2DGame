#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


enum EnemyAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, ATTACK_LEFT, ATTACK_RIGHT, DEAD_LEFT, DEAD_RIGHT
};

void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	direccion = "";
	bJumping = false;
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/soldier.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(18);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.1f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.2f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.3f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.7f, 0.25f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6f, 0.25f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 8);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.1f, 0.5f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.2f, 0.5f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.3f, 0.5f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.4f, 0.5f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.5f, 0.5f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.6f, 0.5f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.1f, 0.5f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.3f, 0.5f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(DEAD_LEFT, 8);
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.1f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.2f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.3f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.4f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.5f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.6f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.7f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.8f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.7f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.8f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.7f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.8f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.7f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.8f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.7f, 0.75f));
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.8f, 0.75f));

	sprite->setAnimationSpeed(DEAD_RIGHT, 8);
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.1f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.2f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.3f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.4f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.6f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.7f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.8f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.7f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.8f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.7f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.8f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.7f, 0.75f)); 
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.8f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.7f, 0.75f));
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.8f, 0.75f));

	sprite->changeAnimation(STAND_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::update(int deltaTime, Player player)
{
	bool acabada = sprite->update(deltaTime);
	if (acabada){
		switch (sprite->animation()){
			case STAND_RIGHT:
				if ((player.posPlayer.x - posEnemy.x) < 34 && (player.posPlayer.x - posEnemy.x) >= 0 && posEnemy[1] == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<3)sprite->changeAnimation(ATTACK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else if ((player.posPlayer.x - posEnemy.x) < 256 && (player.posPlayer.x - posEnemy.x) >= 0 && posEnemy.y == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<2)sprite->changeAnimation(MOVE_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else sprite->changeAnimation(STAND_RIGHT);
				break;
			case STAND_LEFT:
				if ((posEnemy.x - player.posPlayer.x) < 34 && (posEnemy.x - player.posPlayer.x) >= 0 && posEnemy[1] == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<3)sprite->changeAnimation(ATTACK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else if ((posEnemy.x - player.posPlayer.x) < 256 && (posEnemy.x - player.posPlayer.x) >= 0 && posEnemy.y == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<2)sprite->changeAnimation(MOVE_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
			case MOVE_RIGHT:
				if ((player.posPlayer.x - posEnemy.x) < 34 && (player.posPlayer.x - posEnemy.x) >= 0 && posEnemy[1] == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<3)sprite->changeAnimation(ATTACK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else if ((player.posPlayer.x - posEnemy.x) < 256 && (player.posPlayer.x - posEnemy.x) >= 0 && posEnemy.y == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<2)sprite->changeAnimation(MOVE_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else sprite->changeAnimation(STAND_RIGHT);
				break;
			case MOVE_LEFT:
				if ((posEnemy.x - player.posPlayer.x) < 34 && (posEnemy.x - player.posPlayer.x) >= 0 && posEnemy[1] == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<3)sprite->changeAnimation(ATTACK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else if ((posEnemy.x - player.posPlayer.x) < 256 && (posEnemy.x - player.posPlayer.x) >= 0 && posEnemy.y == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<2)sprite->changeAnimation(MOVE_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else sprite->changeAnimation(STAND_RIGHT);
				break;
			case ATTACK_RIGHT:
				if ((player.posPlayer.x - posEnemy.x) < 34 && (player.posPlayer.x - posEnemy.x) >= 0 && posEnemy[1] == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<3)sprite->changeAnimation(ATTACK_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else if ((player.posPlayer.x - posEnemy.x) < 256 && (player.posPlayer.x - posEnemy.x) >= 0 && posEnemy.y == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<2)sprite->changeAnimation(MOVE_RIGHT);
					else sprite->changeAnimation(STAND_RIGHT);
				}
				else sprite->changeAnimation(STAND_RIGHT);
				break;
			case ATTACK_LEFT:
				if ((posEnemy.x - player.posPlayer.x) < 34 && (posEnemy.x - player.posPlayer.x) >= 0 && posEnemy[1] == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<3)sprite->changeAnimation(ATTACK_LEFT);
					else sprite->changeAnimation(STAND_LEFT);
				}
				else if ((posEnemy.x - player.posPlayer.x) < 256 && (posEnemy.x - player.posPlayer.x) >= 0 && posEnemy.y == player.posPlayer.y){
					int mov = rand() % 5;
					if (mov % 5<2)sprite->changeAnimation(MOVE_LEFT);
					else sprite->changeAnimation(STAND_LEFT);
				}
				else sprite->changeAnimation(STAND_LEFT);
				break;
		}
	}

	if (sprite->animation() == MOVE_RIGHT) posEnemy.x += 1;
	if (sprite->animation() == MOVE_LEFT) posEnemy.x -= 1;
	//MODIFICAR POSICIONS


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
