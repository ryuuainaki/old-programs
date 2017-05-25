#include<iostream>
#include<string>
#include<time.h>
#include<Math.h>
#include"SDL.h"
#include"SDL_image.h"
#include"npcSpaceShip.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

npcSpaceShip::npcSpaceShip(int x,int y,int spread,SpaceShip* p,string configFileName):ExternalObject(x,y,spread,p,configFileName) {
	setMass(SHIP_MASS);
	setState(IDLE);
}

void npcSpaceShip::update(Uint8 *keys) {
	float pDistance = sqrt((player->getPositionX()-getPositionX())*(player->getPositionX()-getPositionX())+(player->getPositionY()-getPositionY())*(player->getPositionY()-getPositionY()));
	if (pDistance <= SIGHT_RANGE) setState(ATTACK);
	else setState(IDLE);
	float rise, run;
	rise = getPositionY() - player->getPositionY();
	run = getPositionX() - player->getPositionX();
	float Dir = findAngle(run,rise); //the Theta pointing to the player
	if (Dir < 0) Dir = Dir + (2*PI);
	if (Dir > 2*PI) Dir = Dir - (2*PI);
	float speed = abs(getVelocityX()*sin(getVelTheta()))+abs(getVelocityY()*cos(getVelTheta())), maxSpeed;
	if (getTheta() < Dir) { if (abs(getTheta()-Dir) >= PI) setTheta(getTheta()-TURN); else setTheta(getTheta()+TURN); }
	if (getTheta() > Dir) { if (abs(getTheta()-Dir) >= PI) setTheta(getTheta()+TURN); else setTheta(getTheta()-TURN); }
	switch(getState()) {
		case IDLE:
			setShot(false);
			maxSpeed = .025*MAX_VEL;
			if (Dir > getTheta()-(2*TURN) && Dir < getTheta()+(2*TURN)) { //if moving roughly where you're facing, ...
				if (speed < maxSpeed) addForce(E_SPD,getTheta(),DELTA_T); //...if going too slow, thrust forward
				if (speed > maxSpeed) addForce(E_SPD,getTheta()+PI,DELTA_T); //...if going too fast, thrust backwards
			}
			if (abs(getTheta()-Dir-(2*TURN)) >= PI && abs(getTheta()-Dir+(2*TURN)) >= PI) { //if moving roughly away you're facing, ...
				if (speed < maxSpeed) addForce(E_SPD,getTheta()+PI,DELTA_T); //...if going too slow, thrust backwards
				if (speed > maxSpeed) addForce(E_SPD,getTheta(),DELTA_T); //...if going too fast, thrust forwards
			}
			break;
		case ATTACK:
			if (abs(player->getVelocityX()) < 1 || abs(player->getVelocityY()) < 1) maxSpeed = .05*MAX_VEL;
			else maxSpeed = abs(player->getVelocityX()*sin(player->getVelTheta()))+abs(player->getVelocityY()*cos(player->getVelTheta()));
			if ( Dir > getTheta()-(2*TURN) && Dir < getTheta()+(2*TURN) ) { //if roughly facing towards the player, ...
			   setShot(true);
			   if (speed < maxSpeed) addForce(E_SPD,getTheta(),DELTA_T); //...if going too slow, thrust forward
			   if (speed > maxSpeed) addForce(E_SPD,getTheta()+PI,DELTA_T); //if going too fast, thrust backwards
			}
			else {
				  setShot(false);
				  if (abs(getTheta()-Dir-(2*TURN)) >= PI && abs(getTheta()-Dir+(2*TURN)) >= PI) { //if moving roughly away you're facing, ...
				    if (speed < maxSpeed) addForce(E_SPD,getTheta()+PI,DELTA_T); //...if going too slow, thrust backwards
				    if (speed > maxSpeed) addForce(E_SPD,getTheta(),DELTA_T); //...if going too fast, thrust forwards
				}
			}
			break;
	}
	ExternalObject::update(keys);
}
	
void npcSpaceShip::draw(SDL_Surface* screen) {
	ExternalObject::draw(screen);
}

int npcSpaceShip::getState(){
	return state;
}

void npcSpaceShip::setState(int state){
	this->state = state;
}

void npcSpaceShip::shoot(){
}

