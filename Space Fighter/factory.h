#ifndef FACTORY_H
#define FACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_TTF.h>
#include <SDL_mixer.h>
#include "GameObject.h"
#include "ObjectFactory.h"
#include "SpaceShip.h"
#include "npcSpaceShip.h"
#include "DefendNPC.h"
#include "asteroid.h"
#include "Bullet.h"

using namespace std;

class Factory {
public:
	Factory();
	~Factory();
	vector<GameObject*> loadLevel(int,int,string);
	GameObject* loadObject(string,int,int,int,SpaceShip*);
	void blitText(SDL_Surface*,int,int,string,int);
	Bullet* fireBullet();
	EBullet* fireEBullet(GameObject*);
	Particle* createParticle(int,int,float,float,int,string);
	string configSS;
	string configLVL;
	string configEXT;
	string configGUI;
	string configAST;
	string configNPC;
	string configBLT;
	string configEBT;
	string configMUS;
	string configPARThrust;
protected:
	TTF_Font *font;
	SpaceShip *player;
	ExternalObjectFactory OBJ;
	AsteroidFactory AST; 
	NPCShipFactory NPC;
	DEFShipFactory DEF;
	BulletFactory BLT;
	EBulletFactory EBT;
	ParticleFactory PAR;
};
#endif