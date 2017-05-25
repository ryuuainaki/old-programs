#include<iostream>
#include <fstream>
#include <vector>
#include<string>
#include<time.h>
#include<Math.h>
#include"SDL.h"
#include"SDL_image.h"
#include"SDL_TTF.h"
#include"factory.h"
#include"constants.h"

Factory::Factory() {
	ifstream fin;
	string read;
	char *info,*config, *setting;
	fin.open("./config/pathConfig.txt");
	string SS, AST, NPC, DEF, BLT, EBT, PART;
	while(!fin.eof()) {
		fin >> read;
		info = new char[read.length() + 1];
		strcpy(info,read.c_str());
		if (info != NULL) {
			config = strtok(info,"=");
			setting = strtok(NULL,"=");
			if (strcmp(config,"GUIPATH")==0) { configGUI = setting; }
			if (strcmp(config,"LEVELPATH")==0) { configLVL = setting; }
			if (strcmp(config,"LEVELEXT")==0) { configEXT = setting; }
			if (strcmp(config,"MUS")==0) { configMUS = setting; }
			if (strcmp(config,"SS")==0) { SS = setting; }
			if (strcmp(config,"NPC")==0) { NPC = setting; }
			if (strcmp(config,"AST")==0) { AST = setting; }
			if (strcmp(config,"BLT")==0) { BLT = setting; }
			if (strcmp(config,"EBT")==0) { EBT = setting; }
			if (strcmp(config,"PAR-T")==0) { PART = setting; }
		}
		cout << read << endl;
		delete [] info;
	}
	cout << endl;
	fin.close();
	fin.open(SS);
	configSS = "";
	while(!fin.eof()) { fin >> read; configSS += read; cout << read << endl; configSS += ",";} //converts external file into an internal string, so it only needs to be read in once!
	cout << endl;
	fin.close();
	fin.open(AST);
	configAST = "";
	while(!fin.eof()) {	fin >> read; configAST += read; cout << read << endl; configAST += ",";} //same as above
	cout << endl;
	fin.close();
	fin.open(NPC);
	configNPC = "";
	while(!fin.eof()) { fin >> read; configNPC += read; cout << read << endl; configNPC += ",";} //ditto
	cout << endl;
	fin.close();
	fin.open(BLT);
	configBLT = "";
	while(!fin.eof()) { fin >> read; configBLT += read; cout << read << endl; configBLT += ",";} //ditto
	cout << endl;
	fin.close();
	fin.open(EBT);
	configEBT = "";
	while(!fin.eof()) { fin >> read; configEBT += read; cout << read << endl; configEBT += ",";} //ditto
	cout << endl;
	fin.close();
	fin.open(PART);
	configPARThrust = "";
	while(!fin.eof()) { fin >> read; configPARThrust += read; cout << read << endl; configPARThrust += ",";} //ditto
	cout << endl;
	fin.close();
	TTF_Init();
}
Factory::~Factory() {
	TTF_Quit();
}
vector<GameObject*> Factory::loadLevel(int screenHeight, int screenWidth, string file) {
	ifstream fin;
	vector<GameObject*> objects;
	string ObjName = "N/A";
	int num = 0,x = 0,y = 0,spread = 0;
	SpaceShip* p = new SpaceShip(screenHeight, screenWidth,configSS);
	player = p;
	objects.push_back(p);
	fin.open(file);
	while(!fin.eof()) {
		fin >> ObjName >> num >> x >> y >> spread;
		if (ObjName != "N/A") for (int i=num;i>0;i--) objects.push_back(loadObject(ObjName,x,y,spread,p));
	}
	return objects;
}
GameObject* Factory::loadObject(string ObjName, int x, int y, int spread, SpaceShip* p) {
	if (ObjName == "AST") return AST.create(x,y,spread,p,configAST);
	if (ObjName == "NPC") return NPC.create(x,y,spread,p,configNPC);
	if (ObjName == "DEF") return DEF.create(x,y,spread,p,configNPC);
}

void Factory::blitText(SDL_Surface *screen, int x, int y, string text, int point) {
	font = TTF_OpenFont("./fonts/impact.ttf",point);
	SDL_Rect txtDest = {x,y,0,0};
	SDL_Color color = {255,255,255,0};
	char *txtCopy = new char[text.length()+1];
	strcpy(txtCopy,text.c_str());
	SDL_Surface *txtSur = TTF_RenderText_Solid(font,txtCopy,color);
	SDL_BlitSurface(txtSur,NULL,screen,&txtDest);
}

Bullet* Factory::fireBullet() {
	return BLT.create(player->getPositionX(),player->getPositionY(),0,player,configBLT);
}

EBullet* Factory::fireEBullet(GameObject* obj) {
	return EBT.create(obj->getPositionX(),obj->getPositionY(),0,player,configEBT,obj);
}

Particle* Factory::createParticle(int x,int y,float vX,float vY,int spread,string config) {
	return PAR.create(x,y,vX,vY,spread,player,config);
}