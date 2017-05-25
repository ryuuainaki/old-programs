#include<iostream>
#include<string>
#include"SDL.h"
#include"SDL_image.h"
#include"GameEngine.h"
#include"SpaceShip.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

SpaceShip::SpaceShip(int height, int width, string configFileName):GameObject(configFileName) {
	setDisX(width/2);
	setDisY((3*height)/4);
	setHealth(100);
	setMass(SHIP_MASS);
}
SpaceShip::~SpaceShip() {
	setHealth(0);
}
void SpaceShip::update(Uint8 *keys) {
	if (keys[SDLK_LEFT] == true) {
		setTheta(objTheta-TURN);
		spriteID++;
		if (spriteID < 0) spriteID = numSprites-1;
	}
	if (keys[SDLK_RIGHT] == true) {
		setTheta(objTheta+TURN);
		spriteID--;
		if (spriteID >= numSprites) spriteID = 0;
	}
	if (keys[SDLK_UP] == true) {
		addForce(SPEED,getTheta(),DELTA_T);
	}
	if (keys[SDLK_DOWN] == true) {
		addForce(SPEED,getTheta()+PI,DELTA_T);
	}
	if (getVelocityX() > MAX_VEL) setVelocityX(MAX_VEL);
	if (getVelocityX() < -MAX_VEL) setVelocityX(-MAX_VEL);
	if (getVelocityY() > MAX_VEL) setVelocityY(MAX_VEL);
	if (getVelocityY() < -MAX_VEL) setVelocityY(-MAX_VEL);
}

void SpaceShip::draw(SDL_Surface* screen) {
	applySurface(getDisX(), getDisY(), sheet, screen, &sprites[0]);
}

float SpaceShip::getDisX() {
	return disX;
}

float SpaceShip::getDisY() {
	return disY;
}

void SpaceShip::setDisX(float pX) {
	disX = pX;
}

void SpaceShip::setDisY(float pY) {
	disY = pY;
}