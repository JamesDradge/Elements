/*
=================
cBullets.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cBullets.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cBullets::cBullets() : cSprite()
{
	this->bulletVelocity = { 0.0, 0.0 };
}

/*
=================================================================
Update the sprite position
=================================================================
*/

void cBullets::update(double deltaTime)
{

	FPoint direction = { 0.0, 0.0};
	direction.X = (sin((this->getSpriteRotAngle())*PI/180));
	direction.Y = -(cos((this->getSpriteRotAngle())*PI /180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->bulletVelocity.x = this->bulletVelocity.x + direction.X;
	this->bulletVelocity.y = this->bulletVelocity.y + direction.Y;


	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->bulletVelocity.x * (50.0f *deltaTime);
	currentSpritePos.y += this->bulletVelocity.y * deltaTime;



	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	
	if (this->getSpritePos().x < 1024)
	{
		cout << "Bullet position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	} 

	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
  Sets the velocity for the Bullet
=================================================================
*/
void cBullets::setBulletVelocity(SDL_Point bulletVel)
{
	this->bulletVelocity = bulletVel;
}

/*
=================================================================
  Gets the Bullet velocity
=================================================================
*/
SDL_Point cBullets::getBulletVelocity()
{
	return this->bulletVelocity;
}
