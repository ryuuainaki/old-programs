#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <string>
#include "SDL.h"
#include "GameObject.h"
#include "SpaceShip.h"
#include "ExternalObject.h"
#include "asteroid.h"
#include "npcSpaceShip.h"
#include "DefendNPC.h"
#include "Particle.h"
#include "Bullet.h"

using namespace std;

class ExternalObjectFactory {
public:
	ExternalObjectFactory();
	ExternalObject* create(int,int,int,SpaceShip*,string);
};

class AsteroidFactory: public ExternalObjectFactory {
public:
	AsteroidFactory();
	asteroid* create(int,int,int,SpaceShip*,string);
};

class NPCShipFactory: public ExternalObjectFactory {
public:
	NPCShipFactory();
	npcSpaceShip* create(int,int,int,SpaceShip*,string);
};

class DEFShipFactory: public NPCShipFactory {
public:
	DEFShipFactory();
	DefendNPC* create(int,int,int,SpaceShip*,string);
	DefendNPC* create(int,int,int,SpaceShip*,string,GameObject*);
};

class BulletFactory: public ExternalObjectFactory {
public:
	BulletFactory();
	Bullet* create(int,int,int,SpaceShip*,string);
};

class EBulletFactory: public ExternalObjectFactory {
public:
	EBulletFactory();
	EBullet* create(int,int,int,SpaceShip*,string,GameObject*);
};

class ParticleFactory: public ExternalObjectFactory {
public:
	ParticleFactory();
	Particle* create(int,int,float,float,int,SpaceShip*,string);
};
#endif