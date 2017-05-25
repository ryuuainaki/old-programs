#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>
#include "GameObject.h"

using namespace std;

class SpaceShip: public GameObject {
public:
	SpaceShip(int,int,string);
	~SpaceShip();
	void update(Uint8 *);
	void draw(SDL_Surface*);
	float getDisX();
    float getDisY();
	void setDisX(float);
	void setDisY(float);
protected:
	float disX, disY;
};


#endif