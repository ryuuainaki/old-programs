#ifndef BULLET_H
#define BULLET_H

#include <string>
#include "ExternalObject.h"

using namespace std;

class Bullet: public ExternalObject {
public:
	Bullet(int,int,int,SpaceShip*,string);
	void update(Uint8 *);
	void draw(SDL_Surface*);
private:
	int life;
};

class EBullet: public ExternalObject {
public:
	EBullet(int,int,int,SpaceShip*,string,GameObject*);
	void update(Uint8 *);
	void draw(SDL_Surface*);
private:
	int life;
};

#endif