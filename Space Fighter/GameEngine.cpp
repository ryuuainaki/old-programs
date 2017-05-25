#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include "Timer.h"
#include "GameEngine.h"
#include "constants.h"
#include "ExternalObject.h"
#include "asteroid.h"
#include "npcSpaceShip.h"
#include "SpaceShip.h"
#include "GameObject.h"
#include "ObjectFactory.h"
#include "factory.h"

using namespace std;

GameEngine::GameEngine(){
    SDL_Init(SDL_INIT_EVERYTHING);	
	initializeGUI(fac.configGUI);
	initializeSound(50);
	initializeGameObjects();
	run();
	cleanUpGUI();
}

GameEngine::~GameEngine(){
	for(oiter=objects.begin();oiter!=objects.end();oiter++){
		delete (*oiter);
	}
	Mix_HaltMusic();
	Mix_CloseAudio();
}

void GameEngine::initializeGUI(string guiConfigFile){
	//Initialize values to default values (for compilation reasons)
	screenHeight = 100;
	screenWidth = 100;
	screenBPP = 32;
	frameDelay = 1;
	screen = NULL;
	windowTitle = " ";

	ifstream fin;
	string read;
	char *info, *config, *setting;
	fin.open(guiConfigFile);
	while(!fin.eof()) {
		fin >> read;
		info = new char[read.length() + 1];
		strcpy(info,read.c_str());
		if (info != NULL) {
			config = strtok(info,"=");
			setting = strtok(NULL,"=");
			if (strcmp(config,"SCREENHEIGHT")==0) screenHeight = atoi(setting);
			if (strcmp(config,"SCREENWIDTH")==0) screenWidth = atoi(setting);
			if (strcmp(config,"BPP")==0) screenBPP = atoi(setting);
			if (strcmp(config,"FRAMEDELAY")==0) frameDelay = atoi(setting);
			if (strcmp(config,"NAME")==0) windowTitle = setting;
			if (strcmp(config,"SCREENRFILLKEY")==0) screenR = atoi(setting);
			if (strcmp(config,"SCREENGFILLKEY")==0) screenG = atoi(setting);
			if (strcmp(config,"SCREENBFILLKEY")==0) screenB = atoi(setting);
		}
		delete [] info;
	}

	//Construct Game Window (screen)
    screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBPP, SDL_SWSURFACE);

	//Set Window Title
	SDL_WM_SetCaption(windowTitle.c_str(), NULL );
	charge = BULLET_CHARGE;
	score = 0;
}

void GameEngine::initializeGameObjects(){
	string level1 = fac.configLVL + "1" + fac.configEXT;
	objects = fac.loadLevel(screenHeight,screenWidth,level1);
}

void GameEngine::initializeSound(int chanNum){
	Mix_Music* music = NULL;
	if(Mix_OpenAudio(22050,AUDIO_S16,2,4096)) printf("Unable to load Audio!");
	else {
	   music = Mix_LoadMUS((fac.configMUS).c_str());
	   fire = Mix_LoadWAV("./sound/fire.ogg");
	   hit = Mix_LoadWAV("./sound/hit.wav");
	   thrust = Mix_LoadWAV("./sound/thrust.ogg");
	   Mix_AllocateChannels(chanNum);
	   Mix_VolumeMusic(75);
	   Mix_PlayMusic(music,-1);
	   Mix_SetPanning(-1,0,255);
	}
}

int GameEngine::getScore() {return score;}
void GameEngine::setScore(int pScore) {score = pScore;}

void GameEngine::run(){
	Uint8 *keystrokes;
	bool quit = false;
	radar = true;
	stats = false;
	grav = true;
	clip = false;

	while(quit == false){

		//Start the frame timer
		fps.start();
	
		//Event handling (primitive system here)
		while(SDL_PollEvent(&event)){

			keystrokes = SDL_GetKeyState(NULL);
			if (event.key.type == SDL_KEYDOWN) {
			  if(event.key.keysym.sym == SDLK_r) radar = !radar;
              if(event.key.keysym.sym == SDLK_t) stats = !stats;
			  if(event.key.keysym.sym == SDLK_g) grav = !grav;
			  if(event.key.keysym.sym == SDLK_c) clip = !clip;
			  if(event.key.keysym.sym == SDLK_p) {
				  oiter=objects.begin();
				  (*oiter)->setVelocityX(0.0);
				  (*oiter)->setVelocityY(0.0);
			  }
		    }
			oiter=objects.begin();
			//GUI transmits quit event
			if(event.type == SDL_QUIT || (*oiter)->getDead()){ 
				quit=true;
			}
		}

		physicsPre();

		//Update the game state
		changeGameState(keystrokes);

		physicsPos();

		//Render the game state
		displayGameState();

		//Frame rate equalizer
		if(fps.get_ticks() < frameDelay){
			SDL_Delay(frameDelay - fps.get_ticks());
		}
	}

}

void GameEngine::displayGameState(){
	
	//Fill screen black
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, screenR, screenG, screenB) );

	for(oiter=objects.begin();oiter!=objects.end();oiter++) {
		(*oiter)->draw(screen);
	}

	if (radar) drawRadar(screenWidth-210,screenHeight-210);
	if (stats) drawStats(screenWidth-540,screenHeight-110);

	//Render all blitted images
	SDL_Flip(screen);

}


void GameEngine::changeGameState(Uint8 *keys){
	vector<GameObject*> bullets;
	vector<GameObject*>::iterator player = objects.begin(); 
	float run, rise,angle;
	int dis;
	for(oiter=objects.begin();oiter!=objects.end();) {
		(*oiter)->update(keys);
		if (keys[SDLK_UP] || keys[SDLK_DOWN]) { 
			if (keys[SDLK_UP] && oiter==objects.begin()) {
				playSound(3,0,0,10);
			    bullets.push_back(fac.createParticle((*oiter)->getPositionX(),(*oiter)->getPositionY(),THRUST_SPD*cos((*oiter)->getTheta()),THRUST_SPD*sin((*oiter)->getTheta()),1,fac.configPARThrust));
			}
			if (keys[SDLK_DOWN] && oiter==objects.begin()) {
				playSound(3,0,0,10);
				bullets.push_back(fac.createParticle((*oiter)->getPositionX(),(*oiter)->getPositionY(),THRUST_SPD*cos(-((*oiter)->getTheta())+PI),THRUST_SPD*sin(-((*oiter)->getTheta())+PI),1,fac.configPARThrust)); 
			}
		}
		if(oiter==objects.begin() && keys[SDLK_SPACE]) { 
			if (charge==BULLET_CHARGE) { 
				bullets.push_back(fac.fireBullet());
				playSound(1,0,0,25);
				charge = 0;
			}
			if (charge<BULLET_CHARGE) charge++;
		}
		else {
			if (oiter!=objects.begin() && (*oiter)->getShot()) {
			if (charge==BULLET_CHARGE) { 
				bullets.push_back(fac.fireEBullet(*oiter));
				run = (*player)->getPositionX()-(*oiter)->getPositionX(); 
			    rise = (*player)->getPositionY()-(*oiter)->getPositionY();
				dis = sqrt(run*run+rise*rise);
				angle = (*oiter)->findAngle(run,rise)-(*player)->getTheta();
				playSound(1,angle,dis,25);
				charge = 0;
			}
	     	if (charge<BULLET_CHARGE) charge++;
			}
		}
		if ((*oiter)->getDead()) {objects.erase(oiter);} 
		else oiter++;
	}
	for(oiter=bullets.begin();oiter!=bullets.end();oiter++)
		objects.push_back(*oiter);
}


void GameEngine::cleanUpGUI(){
    //Shut down SDL
    SDL_Quit();
}

void GameEngine::drawRadar(int x, int y) {
	const int RADAR_SCALE = 15;
	SDL_Rect outerBox = {x-5,y-5,210,210};
	SDL_Rect innerBox = {x,y,200,200};
	SDL_Rect player1 = {x+98,y+98,3,3};
	vector<SDL_Rect*> enemy;
	int pX, pY;
	float pT;
	int i = 0;
	SDL_FillRect( screen, &outerBox, SDL_MapRGB( screen->format, 0, 100, 255) );
	SDL_FillRect( screen, &innerBox, SDL_MapRGB( screen->format, 0, 0, 0) );
	for(oiter=objects.begin();oiter!=objects.end();oiter++,i++) {
		enemy.push_back(new SDL_Rect);
		if (oiter==objects.begin()) {
			SDL_FillRect( screen, &player1, SDL_MapRGB( screen->format, 255, 0, 0) );
			pX = (*oiter)->getPositionX();
			pY = (*oiter)->getPositionY();
			pT = (*oiter)->getTheta();
		}
		else {
			if (!(*oiter)->getClip()) {
			int dX = (pX-(*oiter)->getPositionX())/RADAR_SCALE;
			int dY = ((*oiter)->getPositionY()-pY)/RADAR_SCALE;
			float theta = pT;
			if (theta < 0) theta = theta + (2*PI);
		    if (theta > 2*PI) theta = theta - (2*PI);
			float rX = (dX*cos(theta)-dY*sin(theta));
			float rY = (dY*cos(theta)+dX*sin(theta));
			enemy[i]->w = enemy[i]->h = 2;
			if (rX >= -100 && rX <= 100) enemy[i]->x = rX + x+99;
			if (rX < -100) enemy[i]->x = x-1;
			if (rX > 100) enemy[i]->x = x+199;
		    if (rY >= -100 && rY <= 100) enemy[i]->y = rY + y+99;
			if (rY < -100) enemy[i]->y = y-1;
			if (rY > 100) enemy[i]->y = y+199;
			int color = 355-abs(rX)-abs(rY);
			if (color < 0) color = 0;
			if (color > 255) color = 255;
			SDL_FillRect( screen, enemy[i], SDL_MapRGB( screen->format, color, color, color) );
			}
		}
	}
}

void GameEngine::drawStats(int x, int y) {
	SDL_Rect outerBox = {x-5,y-5,320,110};
	SDL_Rect innerBox = {x,y,310,100};
	SDL_FillRect( screen, &outerBox, SDL_MapRGB( screen->format, 0, 100, 255) );
	SDL_FillRect( screen, &innerBox, SDL_MapRGB( screen->format, 0, 0, 0) );
	oiter=objects.begin();
	char *cX = new char[10];
	char *cY = new char[10];
	char *cS = new char[8];
	char *cG = new char[2];
	char *cC = new char[2];
	itoa(((*oiter)->getPositionX())/100,cX,10);
    itoa(((*oiter)->getPositionY())/100,cY,10);
	itoa(score,cS,8);
	itoa(grav,cG,2);
	itoa(clip,cC,2);
	string location = "Loc: ("+string(cX)+","+string(cY)+")";
	string score = "Score: "+string(cS);
	string gtoggle = "G["+string(cG)+"]";
	string ctoggle = "C["+string(cC)+"]";
	fac.blitText(screen,x+10,y+30,location,20);
	fac.blitText(screen,x+10,y+60,score,20);
	fac.blitText(screen,x+200,y+30,gtoggle,20);
	fac.blitText(screen,x+235,y+30,ctoggle,20);
	SDL_Rect *healthBar = new SDL_Rect;
	healthBar->x = x+5;
	healthBar->y = y+5;
	healthBar->w = (((*oiter)->getHealth())*3)%301;
	healthBar->h = 20;
	SDL_FillRect( screen, healthBar, SDL_MapRGB( screen->format, (100-(*oiter)->getHealth())*2.55, (*oiter)->getHealth()*2.55, 0) );
}

void GameEngine::physicsPre() {
	if (grav) {
		float m1,m2,rX,rY,a1,a2,b1,b2,f,d;
		int x1,x2,y1,y2;
	    vector<GameObject*>::iterator o1, o2;
		for (o1=objects.begin();o1!=objects.end();o1++) {
			for (o2=objects.begin();o2!=objects.end();o2++) {
				x1 = (*o1)->getPositionX();
				x2 = (*o2)->getPositionX();
				y1 = (*o1)->getPositionY();
				y2 = (*o2)->getPositionY();
				a1 = (*o1)->getSpriteWidth();
				b1 = (*o1)->getSpriteHeight();
				a2 = (*o2)->getSpriteWidth();
				b2 = (*o2)->getSpriteHeight();
				rX = (x2+(a2/2))-(x1+(a1/2));
				rY = (y2+(b2/2))-(y1+(b1/2));
				float r = sqrtf(powf(abs(rX),2)+powf(abs(rY),2));
				if (o1!=o2 && (r<=G_RANGE && r!=0)) {
					m1 = (*o1)->getMass();
					m2 = (*o2)->getMass();
					f = G*((m1*m2)/(r*r));
					if (o1==objects.begin()) d = (*o1)->findAngle(rX,rY);
					else d = ((*o1)->findAngle(rX,rY))-PI;
					if (d < 0) d = d + (2*PI);
					if (d > 2*PI) d = d - (2*PI);
					(*o1)->addForce(f,d,DELTA_T);
				}
			}
		}
	}
}

void GameEngine::physicsPos() {
	vector<GameObject*>::iterator o1,o2;
	float x1,x2,y1,y2,ox,oy,rX,rY,orX,orY,d,r,or,a1,b1,a2,b2,nX,nY,n,fX,fY,f,br1,br2,dT=1;
	for (o1=objects.begin();o1!=objects.end();o1++) {
		a1 = (*o1)->getSpriteWidth()/2; //Half Sprite Width of Object 1
		b1 = (*o1)->getSpriteHeight()/2; //Half Sprite Height of Object 1
		ox = (*o1)->getPositionX(); //old X position of object 1
		oy = (*o1)->getPositionY(); //old Y position of object 1
		(*o1)->move(DELTA_T,o1==objects.begin());
		if (clip && !(*o1)->getClip()) {
			x1 = (*o1)->getPositionX(); //new X position of object 1
			y1 = (*o1)->getPositionY(); //new Y position of object 1
			(*o1)->setPositionX(ox); //move back to old X
			(*o1)->setPositionY(oy); //move back to old Y
			d = (*o1)->getForceDir(); //Theta of current movement
			for (o2=objects.begin();o2!=objects.end();o2++) {
				a2 = (*o2)->getSpriteWidth()/2; //Half Sprite Width of Object 2
				b2 = (*o2)->getSpriteHeight()/2; //Half Sprite Height of Object 2
				x2 = (*o2)->getPositionX(); //X position of object 2
				y2 = (*o2)->getPositionY(); //Y position of object 2
				rX = (x1+a1)-(x2+a2); //X-distance between objects
				rY = (y1+b1)-(y2+b2); //Y-distance between objects
				orX = (ox+a1)-(x2+a2); //old X-distance between objects
				orY = (oy+b1)-(x2+b2); //old Y-distance between objects
				r = sqrtf(powf(rX,2)+powf(rY,2)); //distance between objects
				or = sqrtf(powf(orX,2)+powf(orY,2)); //old distance between objects
				nX = ((x2+a2)-(x1+a1))/r; //X of N vector
				nY = ((y2+b1)-(y1+b2))/r; //Y of N vector
				br1 = (a1*b1)/sqrtf(powf(b1*cos(d),2)+powf(a1*sin(d),2)); //collision radius of object 1 (ellipsal collision)
				br2 = (a2*b2)/sqrtf(powf(b2*cos(d),2)+powf(a2*sin(d),2)); //collision radius of object 2 (ellipsal collision)
				if (o1!=o2 && r<=(br1+br2)) {
					fX = nX*((1+E)*(((*o1)->getMass()+(*o2)->getMass())*((*o1)->getForceMag()-(*o2)->getForceMag())*nX/((*o1)->getMass()+(*o2)->getMass())));
					fY = nY*((1+E)*(((*o1)->getMass()+(*o2)->getMass())*((*o1)->getForceMag()-(*o2)->getForceMag())*nY/((*o1)->getMass()+(*o2)->getMass())));
					f = sqrt(fX*fX+fY*fY);
					(*o1)->setForce(f,d+PI,1);
					(*o1)->modHealth(f/DELTA_T);
					setScore(getScore()+(f/DELTA_T));
					(*o2)->setForce(f,d,1);
					(*o2)->modHealth(f/DELTA_T);
					setScore(getScore()+(f/DELTA_T));
					playSound(2,0,r,100);
				}
			}
			(*o1)->move(DELTA_T,o1==objects.begin());
		}
	}
}

void GameEngine::playSound(int sound, float theta, int distance, int volume) {
	if (distance > 255) distance = 255;
	int direction = theta*TO_DEGREES;
	noise = NULL;
	if (sound == 1) noise = fire;
	if (sound == 2) noise = hit;
	if (sound == 3) noise = thrust;
	Mix_SetPosition(-1,direction,distance);
	Mix_VolumeChunk(noise,volume);
	Mix_PlayChannel(-1,noise,0);
}