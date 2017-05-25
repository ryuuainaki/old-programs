#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<string>
#include"SDL.h"
#include"SDL_image.h"
#include"SDL_mixer.h"

using namespace std;

class GameObject{

public:
	
	GameObject(string);
	~GameObject();
	virtual void update(Uint8 *)=0;
	virtual void draw(SDL_Surface*)=0;

	float getPositionX();
	float getPositionY();
	float getVelocityX();
	float getVelocityY();
	float getTheta();
	float getVelTheta();
	float getMass();
	float getForceMag();
	float getForceDir();
	int getSpriteID();
	int getSpriteHeight();
	int getSpriteWidth();
	int getNumSprites();
	int getHealth();
	bool getDead();
	bool getClip();
	bool getShot();

	void setPositionX(float);
	void setPositionY(float);
	void setVelocityX(float);
	void setVelocityY(float);
	void setTheta(float);
	void setMass(float);
	void setSpriteID(int);
	void setHealth(int);
	void modHealth(int);
	void setForce(float,float,float);
	void addForce(float,float,float);
	void setShot(bool);
	void move(float,bool);
	float findAngle(float,float);

protected:

	//Game State
	float positionX;
	float positionY;
	float velocityX;
	float velocityY;
	float objTheta;
	float velTheta;
	float mass;
	int charge;
	int health;
	bool dead;
	bool noclip;
	bool shoot;

	//Intermediate (Engine/Art interface)
	int spriteID;
	int spriteHeight;
	int spriteWidth;

	//Art Assets
	SDL_Surface* sheet;   //source of object's art
	int numSprites;    
	SDL_Rect* sprites;

	//SDL helper functions
	SDL_Surface* loadImage(string filename, int,int,int);
	void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
};

#endif