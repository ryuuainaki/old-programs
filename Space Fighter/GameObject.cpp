#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include"SDL.h"
#include"SDL_mixer.h"
#include<Math.h>
#include"SDL_image.h"
#include"GameObject.h"
#include"constants.h"

using namespace std;

GameObject::GameObject(string configInfo){
	//Declare local variables
	string spriteSheetFileName;
	int nrows, ncols, mapR(0), mapG(0), mapB(0);
	vector<char*> token;
	char *info, *input, *config, *setting;
	info = new char[configInfo.length() + 1];
	strcpy(info,configInfo.c_str());
	input = strtok(info,",");
	for (int i=0;input!=NULL;i++) { token.push_back(input); input = strtok(NULL,",");}
	for (int i=0;i<token.size();i++) {
		if (token[i] != NULL) {
			config = strtok(token[i],"=");
			setting = strtok(NULL,"=");
			if (strcmp(config,"IMAGEPATH")==0) spriteSheetFileName = setting; //art asset
			if (strcmp(config,"HEIGHT")==0) spriteHeight = atoi(setting);	  //sprite height (pixels)
			if (strcmp(config,"WIDTH")==0) spriteWidth = atoi(setting);       //sprite width (pixels)
			if (strcmp(config,"NUMSPRITES")==0) numSprites = atoi(setting);   //total number of sprites in sheet
			if (strcmp(config,"NROWS")==0) nrows = atoi(setting);             //num sprite sheet rows
			if (strcmp(config,"NCOLS")==0) ncols = atoi(setting);			  //num sprite sheet columns
			if (strcmp(config,"RMAPKEY")==0) mapR = atoi(setting);			  //R color code to make transparent in sprite
			if (strcmp(config,"GMAPKEY")==0) mapG = atoi(setting);			  //G color code to make transparent in sprite
			if (strcmp(config,"BMAPKEY")==0) mapB = atoi(setting);			  //B color code to make transparent in sprite
		}
	}		
	delete [] info;

	//Load Object Specific Art data
	sheet = loadImage(spriteSheetFileName,mapR,mapG,mapB);  //from file

	//Allocate Clips
	sprites = new SDL_Rect[numSprites];

	//Map the sprite angle to the spritesheet
	for(int i=0;i<numSprites;i++){
		int row = i/ncols;
		int col = i%ncols;
		sprites[i].x = col*spriteWidth;
		sprites[i].y = row*spriteHeight;
		sprites[i].w = spriteWidth;
		sprites[i].h = spriteHeight;
	}

	//Assign state information	
	positionX = 0;
	positionY = 0;
	velocityX = 0;
	velocityY = 0;
	objTheta = 0;
	velTheta = 0;
	spriteID = 0;
	charge = BULLET_CHARGE;
	health = 1;
	shoot = false;
	dead = false;
	noclip = false;
}

//Destructor
GameObject::~GameObject(){
	SDL_FreeSurface(sheet);
}

//Getter Methods
float GameObject::getPositionX(){
	return(positionX);
}

float GameObject::getPositionY(){
	return(positionY);
}

float GameObject::getVelocityX(){
	return(velocityX);
}

float GameObject::getVelocityY(){
	return(velocityY);
}

float GameObject::getTheta(){
	return(objTheta);
}

float GameObject::getVelTheta(){
	return(velTheta);
}

float GameObject::getMass(){
	return(mass);
}

int GameObject::getSpriteID(){
	return(spriteID);
}

int GameObject::getSpriteHeight(){
	return(spriteHeight);
}

int GameObject::getSpriteWidth(){
	return(spriteWidth);
}

int GameObject::getNumSprites(){
	return(numSprites);
}

int GameObject::getHealth(){
	return(health);
}

bool GameObject::getDead(){
	return(dead);
}

bool GameObject::getClip(){
	return(noclip);
}

bool GameObject::getShot(){
	return(shoot);
}

//Setter Methods
void GameObject::setPositionX(float positionX){
	this->positionX = positionX;
}

void GameObject::setPositionY(float positionY){
	this->positionY = positionY;
}

void GameObject::setVelocityX(float velocityX){
	this->velocityX = velocityX;
	float theta = atan2(this->velocityY,this->velocityX);
    if (theta < 0) theta = theta + (2*PI);
	if (theta > 2*PI) theta = theta - (2*PI);
	this->velTheta = theta;
}

void GameObject::setVelocityY(float velocityY){
	this->velocityY = velocityY;
	float theta = atan2(this->velocityY,this->velocityX);
    if (theta < 0) theta = theta + (2*PI);
	if (theta > 2*PI) theta = theta - (2*PI);
	this->velTheta = theta;
}

void GameObject::setMass(float mass){
		this->mass = mass;
}

void GameObject::setSpriteID(int spriteID){
		this->spriteID = spriteID;
}

void GameObject::setHealth(int health){
		this->health = health;
		if (health <= 0) dead = true;
}

void GameObject::modHealth(int health){
		this->health += health;
		if (health <= 0) dead = true;
}

void GameObject::setTheta(float theta){
	if (theta < 0) theta = theta + (2*PI);
	if (theta > 2*PI) theta = theta - (2*PI);
	this->objTheta = theta;
}

float GameObject::getForceMag(){
	return (sqrtf(powf(abs(getVelocityX()),2)+powf(abs(getVelocityY()),2)))*getMass();
}
float GameObject::getForceDir(){
	return velTheta;
}
void GameObject::setForce(float Mag,float Dir,float dT){
	float theta = Dir;
	if (theta < 0) theta = theta + (2*PI);
	if (theta > 2*PI) theta = theta - (2*PI);
	setVelocityX((-Mag/(this->getMass())*sin(theta))*dT);
	setVelocityY((-Mag/(this->getMass())*cos(theta))*dT);
}
void GameObject::addForce(float Mag,float Dir,float dT){
	float theta = Dir;
	if (theta < 0) theta = theta + (2*PI);
	if (theta > 2*PI) theta = theta - (2*PI);
	setVelocityX(getVelocityX()+(-Mag/(this->getMass())*sin(theta))*dT);
	setVelocityY(getVelocityY()+(-Mag/(this->getMass())*cos(theta))*dT);
}
void GameObject::setShot(bool shot){
		this->shoot = shot;
}
void GameObject::move(float dT,bool player) {
	if (player) setPositionX(getPositionX()-getVelocityX());
	else setPositionX(getPositionX()+getVelocityX());
	setPositionY(getPositionY()+getVelocityY());
}

SDL_Surface* GameObject::loadImage(string fileName, int mapR, int mapG, int mapB ){
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( fileName.c_str() );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, mapR, mapG, mapB);
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
	}
    return optimizedImage;

}

void GameObject::applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

float GameObject::findAngle(float x, float y) {
	float angle;
	if (x==0) x=0.00001;
	if (x > 0 && y >= 0) angle = .5*PI - atan(abs(y/x));
	if (x > 0 && y < 0) angle = .5*PI + atan(abs(y/x));
	if (x < 0 && y < 0) angle = 1.5*PI - atan(abs(y/x));
    if (x < 0 && y >= 0) angle = 1.5*PI + atan(abs(y/x));
	return angle;
}