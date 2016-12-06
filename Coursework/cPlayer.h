/*
=================
cPlayer.h
- Header file for class definition - SPECIFICATION
- Header file for the Player class which is a child of cSprite class
=================
*/
#ifndef _CROCKET_H
#define _CROCKET_H
#include "cSprite.h"

class cPlayer : public cSprite
{
private:
	SDL_Point playerVelocity;

public:
	cPlayer();
	void update(double deltaTime);		// Player update method
	void setPlayerVelocity(SDL_Point playerVel);   // Sets the velocity for the player
	SDL_Point getPlayerVelocity();				 // Gets the player velocity
};
#endif