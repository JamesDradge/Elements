/*
=================
cEnemy.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cEnemy.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cEnemy::cEnemy() : cSprite()
{
	this->enemyVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cEnemy::update(double deltaTime)
{

	this->setSpriteRotAngle(this->getSpriteRotAngle() +(0.0f * deltaTime)); 
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() -360);
	}

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	cout << "Enemy position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Enemy
=================================================================
*/
void cEnemy::setEnemyVelocity(SDL_Point EnemyVel)
{
	enemyVelocity = EnemyVel;
}
/*
=================================================================
  Gets the Enemy velocity
=================================================================
*/
SDL_Point cEnemy::getEnemyVelocity()
{
	return enemyVelocity;
}
