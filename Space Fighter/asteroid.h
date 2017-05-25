#ifndef ASTEROID_H
#define ASTEROID_H

#include <string>
#include "ExternalObject.h"

using namespace std;

class asteroid: public ExternalObject {
public:
	asteroid(int,int,int,SpaceShip*,string);
	int getRotation();
	int getFPR();
	void setRotation(int);
	void setFPR(int);
	void update(Uint8 *);
	void draw(SDL_Surface*);
private:
	int rotMag;
	bool rotNeg;
	int fpr; //Frames Per Rotation
};

#endif