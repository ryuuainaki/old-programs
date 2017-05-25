#ifndef EXTERNALOBJECT_H
#define EXTERNALOBJECT_H

#include <string>
#include "SpaceShip.h"
#include "GameObject.h"

using namespace std;

class ExternalObject: public GameObject {
public:
	ExternalObject(int,int,int,SpaceShip*,string);
	float getDisX();
	float getDisY();
	void setDisX(float);
	void setDisY(float);
	void update(Uint8 *);
	void draw(SDL_Surface*);
protected:
	SpaceShip* player;
	float disX, disY;
};
#endif