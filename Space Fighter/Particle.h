#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include "ExternalObject.h"

using namespace std;

class Particle: public ExternalObject {
public:
	Particle(int,int,float,float,int,SpaceShip*,string);
	void update(Uint8 *);
	void draw(SDL_Surface*);
private:
	int life;
};

#endif