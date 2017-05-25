#include<iostream>
#include<string>
#include<time.h>
#include<Math.h>
#include"SDL.h"
#include"SDL_image.h"
#include"Bullet.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

Bullet::Bullet(int x,int y,int spread,SpaceShip* p,string configFileName):ExternalObject(x,y,spread,p,configFileName) {
	life = BULLET_LIFE;
	noclip = true;
	setPositionX(p->getPositionX());
	setPositionY(p->getPositionY());
	setVelocityX(p->getVelocityX());
	setVelocityY(p->getVelocityY());
	setTheta(-(p->getTheta())+PI);
	setForce(BULLET_SPD,getTheta(),DELTA_T);
}

void Bullet::update(Uint8 *keys) {
	ExternalObject::update(keys);
	life--;
	if (life <= BULLET_LIFE-12) noclip = false;
	if(life <= 0) dead = true;
}

void Bullet::draw(SDL_Surface* screen) {
	if(!dead || life==BULLET_LIFE) ExternalObject::draw(screen);
}

EBullet::EBullet(int x,int y,int spread,SpaceShip* p,string configFileName,GameObject* obj):ExternalObject(x,y,spread,p,configFileName) {
	setTheta(obj->getTheta()+PI);
	life = BULLET_LIFE;
	noclip = true;
	setPositionX(obj->getPositionX()+(obj->getSpriteWidth()/2));
	setPositionY(obj->getPositionY()+(obj->getSpriteHeight()/2));
	setVelocityX(obj->getVelocityX());
	setVelocityY(obj->getVelocityY());
	setForce(BULLET_SPD,getTheta(),DELTA_T);
}

void EBullet::update(Uint8 *keys) {
	ExternalObject::update(keys);
	life--;
	if (life <= BULLET_LIFE-12) noclip = false;
	if(life <= 0) dead = true;
}

void EBullet::draw(SDL_Surface* screen) {
	if(!dead || life==BULLET_LIFE) ExternalObject::draw(screen);
}