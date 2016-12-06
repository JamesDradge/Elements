/*
=================
cBullets.h
- Header file for class definition - SPECIFICATION
- Header file for the Bullet class which is a child of cSprite class
=================
*/
#ifndef _cBullets_H
#define _cBullets_H
#include "cSprite.h"


class cBullets : public cSprite
{
private:
	SDL_Point bulletVelocity;


public:
	cBullets();
	void update(double deltaTime);		// Bullet update method
	void setBulletVelocity(SDL_Point bulletVel);   // Sets the velocity for the Bullet
	SDL_Point getBulletVelocity();				 // Gets the Bullet velocity
};
#endif