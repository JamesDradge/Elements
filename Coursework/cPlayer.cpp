
//cPlayer.cpp
//Header file for class definition - IMPLEMENTATION

#include "cPlayer.h"


//Defualt Constructor

cPlayer::cPlayer() : cSprite()
{
	this->playerVelocity = { 0.0f, 0.0f };
}

//Update the sprite position


void cPlayer::update(double deltaTime)
{

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	//this->playerVelocity.x = this->playerVelocity.x + direction.X;
	//this->playerVelocity.y = this->playerVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x;
	currentSpritePos.y += this->getSpriteTranslation().y;

	//this->playerVelocity.x *= 0.95;
	//this->playerVelocity.y *= 0.95;
	
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());
	this->setSpriteTranslation({ 0, 0 });
}

//Sets the velocity for the player

void cPlayer::setPlayerVelocity(SDL_Point playerVel)
{
	playerVelocity = playerVel;
}


//Gets the player velocity


SDL_Point cPlayer::getPlayerVelocity()
{
	return playerVelocity;
}


