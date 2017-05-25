#include<iostream>
#include<string>
#include<time.h>
#include<Math.h>
#include"SDL.h"
#include"SDL_image.h"
#include"ExternalObject.h"
#include"SpaceShip.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

ExternalObject::ExternalObject(int x,int y,int spread,SpaceShip* p,string configFileName):GameObject(configFileName){
    float nX = (x-spread) + float(((2*spread)*rand())/(RAND_MAX+1.0));
	float nY = (y-spread) + float(((2*spread)*rand())/(RAND_MAX+1.0));
	float nT = float((rand()%72)*TURN);
	setPositionX(nX);
	setPositionY(nY);
	setTheta(nT);
	player = p;
	setDisX((p->getPositionX()+(p->getSpriteWidth()/2))+nX);
	setDisY((-p->getPositionY()+(p->getSpriteHeight()/2))+nY);
}

float ExternalObject::getDisX() {
	return disX;
}

float ExternalObject::getDisY() {
	return disY;
}

void ExternalObject::setDisX(float pX) {
	disX = pX; 
}

void ExternalObject::setDisY(float pY) {
	disY = pY;
}

void ExternalObject::update(Uint8 *keys) {
	float x = -getPositionX();
	float y = -getPositionY();
	float oX = -player->getPositionX();
    float oY = -player->getPositionY();
	float theta = player->getTheta();
	setDisX((x-oX)*cos(theta)-(oY-y)*sin(theta));
	setDisY((oY-y)*cos(theta)+(x-oX)*sin(theta));
	theta = objTheta + player->getTheta();
    if (theta < 0) theta = theta + (2*PI);
	if (theta > 2*PI) theta = theta - (2*PI);
	int sID = (int)floor(theta*(numSprites/(2*PI)));
	if (sID >= numSprites) sID-=(numSprites);
	if (sID < 0) sID+=(numSprites-1);
	setSpriteID(sID);
}

void ExternalObject::draw(SDL_Surface* screen) {
	float x = (player->getDisX()+(player->getSpriteWidth()/2))+getDisX();
	float y = (player->getDisY()+(player->getSpriteHeight()/2))+getDisY();
	if (x>-spriteWidth && x<screen->w)
		if (y>-spriteHeight && y<screen->h) 
			applySurface(x,y,sheet,screen,&sprites[spriteID]);
}