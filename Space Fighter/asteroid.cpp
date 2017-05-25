#include<iostream>
#include<string>
#include<time.h>
#include<Math.h>
#include"SDL.h"
#include"SDL_image.h"
#include"asteroid.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

asteroid::asteroid(int x,int y,int spread,SpaceShip* p,string configFileName):ExternalObject(x,y,spread,p,configFileName) {
	setRotation(11);
	setFPR(FPR_MAX-rotMag);
	setMass(AST_MASS);
}

int asteroid::getRotation() {
	return (-1*rotNeg)*(rotMag);
}

int asteroid::getFPR() {
	return fpr;
}

void asteroid::setRotation(int nRot) { 
	if (nRot >= -5 && nRot < 0) { rotNeg = 1; rotMag = abs(nRot); } 
    if (nRot >= 0 && nRot <= 5) { rotNeg = 0; rotMag = abs(nRot); } 
	if (nRot < -5 || nRot > 5) { rotNeg = rand()%2; rotMag = rand()%6; }
}

void asteroid::setFPR(int nFPR) { 
	fpr = nFPR;
}
	
void asteroid::update(Uint8 *keys) {
	if (rotMag != 0) {
		float astTurn = (2*PI)/numSprites;
		fpr--;
		if (fpr == 0 ) {
			if (rotNeg) setTheta(getTheta()-(astTurn));
			else setTheta(getTheta()+(astTurn));
			setFPR(FPR_MAX-rotMag);
		}
	}
	ExternalObject::update(keys);
}
	
void asteroid::draw(SDL_Surface* screen) {
	ExternalObject::draw(screen);
}