#include<iostream>
#include<string>
#include<time.h>
#include<Math.h>
#include"SDL.h"
#include"SDL_image.h"
#include"Particle.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

Particle::Particle(int x,int y,float vX,float vY,int spread,SpaceShip* p,string configFileName):ExternalObject(x,y,0,p,configFileName) {
	life = numSprites-1;
	setMass(1);
    noclip = true;
	float rX = spread - float((((2*spread)*rand())/(RAND_MAX+1.0)));
	float rY = spread - float((((2*spread)*rand())/(RAND_MAX+1.0)));
	setVelocityX(rX+vX);
	setVelocityY(rY+vY);
}

void Particle::update(Uint8 *keys) {
	ExternalObject::update(keys);
	life--;
	setSpriteID((numSprites-1)-life);
	if (life <= 0) dead = true;
}

void Particle::draw(SDL_Surface* screen) {
	ExternalObject::draw(screen);
}