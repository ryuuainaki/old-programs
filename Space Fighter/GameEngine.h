#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>
#include "Timer.h"
#include "GameObject.h"
#include "factory.h"

using namespace std;

class GameEngine{

public:
	GameEngine();
	~GameEngine();
	int getScore();
	void setScore(int);

private:

	//Major helper functions
	void initializeGUI(string);
	void initializeGameObjects();
	void initializeSound(int);
	void run();
	void cleanUpGUI();
	void drawRadar(int,int);
	void drawStats(int,int);
	void physicsPre();
	void physicsPos();
	void playSound(int,float,int,int);

	//State update
	void changeGameState(Uint8*);

	//State presentation
	void displayGameState();

	//GUI/Engine Constructs
	int screenHeight;
	int screenWidth;
	int screenBPP;
	int frameDelay;
	int screenR;
	int screenG;
	int screenB;
	int charge;
	string windowTitle;
	SDL_Event event;
	SDL_Surface* screen;
	Timer fps;
	bool radar;
	bool stats;
	bool grav;
	bool clip;
	int score;

	//Game Elements
	Factory fac;
	BulletFactory bullet;
	vector<GameObject*> objects;  
	vector<GameObject*> oldObjects;
	vector<GameObject*>::iterator oiter;
	Mix_Music* music;
	Mix_Chunk* noise;
	Mix_Chunk* fire;
	Mix_Chunk* hit;
	Mix_Chunk* thrust;
};

#endif