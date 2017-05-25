#ifndef NPCSPACESHIP_H
#define NPCSPACESHIP_H

#include <string>
#include "ExternalObject.h"

using namespace std;

class npcSpaceShip: public ExternalObject {
public:
	npcSpaceShip(int,int,int,SpaceShip*,string);
	int getState();
	void setState(int);
	void update(Uint8 *);
	void draw(SDL_Surface*);
	void shoot();
private:
	int state;
};
#endif