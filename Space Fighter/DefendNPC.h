#ifndef DEFENDNPC_H
#define DEFENDNPC_H

#include <string>
#include "ExternalObject.h"
#include "npcSpaceShip.h"

using namespace std;

class DefendNPC: public npcSpaceShip {
public:
	DefendNPC(int,int,int,SpaceShip*,string);
	DefendNPC(int,int,int,SpaceShip*,string,GameObject*);
	void update(Uint8 *);
	void draw(SDL_Surface*);
private:
	GameObject* guard;
};

class GuardPoint: public ExternalObject {
public:
	GuardPoint(int,int,int,SpaceShip*,string);
	void update(Uint8 *);
	void draw(SDL_Surface*);
};
#endif