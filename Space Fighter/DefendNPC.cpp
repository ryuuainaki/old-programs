#include<iostream>
#include<string>
#include<time.h>
#include<Math.h>
#include"SDL.h"
#include"SDL_image.h"
#include"DefendNPC.h"
#include"npcSpaceShip.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

DefendNPC::DefendNPC(int x,int y,int spread,SpaceShip* p,string configFileName):npcSpaceShip(x,y,spread,p,configFileName) {
	guard = new GuardPoint(x,y,0,p,configFileName);
}

DefendNPC::DefendNPC(int x,int y,int spread,SpaceShip* p,string configFileName,GameObject* obj):npcSpaceShip(x,y,spread,p,configFileName) {
	guard = obj;
}

void DefendNPC::update(Uint8 *keys) {
	float pDistance = sqrt((player->getPositionX()-getPositionX())*(player->getPositionX()-getPositionX())+(player->getPositionY()-getPositionY())*(player->getPositionY()-getPositionY()));
	float lDistance = sqrt((guard->getPositionX()-getPositionX())*(guard->getPositionX()-getPositionX())+(guard->getPositionY()-getPositionY())*(guard->getPositionY()-getPositionY()));
	if (pDistance <= SIGHT_RANGE && lDistance <= GUARD_RADIUS) setState(ATTACK);
	else setState(IDLE);
	float rise, run;
	if (getState() == IDLE) {
		rise = getPositionY() - guard->getPositionY();
		run = getPositionX() - guard->getPositionX();
	}
	if (getState() == ATTACK) {
		rise = getPositionY() - player->getPositionY();
		run = getPositionX() - player->getPositionX();
	}
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
	
void DefendNPC::draw(SDL_Surface* screen) {
	ExternalObject::draw(screen);
}

GuardPoint::GuardPoint(int x,int y,int spread,SpaceShip* p,string configFileName):ExternalObject(x,y,spread,p,configFileName) {
}

void GuardPoint::update(Uint8 *keys) {
	ExternalObject::update(keys);
}
	
void GuardPoint::draw(SDL_Surface* screen) {
	ExternalObject::draw(screen);
}