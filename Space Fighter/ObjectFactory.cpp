#include <string>
#include "GameObject.h"
#include "ExternalObject.h"
#include "asteroid.h"
#include "npcSpaceShip.h"
#include "DefendNPC.h"
#include "Bullet.h"
#include "ObjectFactory.h"

ExternalObjectFactory::ExternalObjectFactory() {}
ExternalObject* ExternalObjectFactory::create(int x,int y,int spread,SpaceShip* p, string configFileName) {
	return new ExternalObject(x,y,spread,p,configFileName);
}
AsteroidFactory::AsteroidFactory() {}
asteroid* AsteroidFactory::create(int x,int y,int spread,SpaceShip* p, string configFileName) {
	return new asteroid(x,y,spread,p,configFileName);
}
NPCShipFactory::NPCShipFactory() {}
npcSpaceShip* NPCShipFactory::create(int x,int y,int spread,SpaceShip* p, string configFileName) {
	return new npcSpaceShip(x,y,spread,p,configFileName);
}
DEFShipFactory::DEFShipFactory() {}
DefendNPC* DEFShipFactory::create(int x,int y,int spread,SpaceShip* p, string configFileName) {
	return new DefendNPC(x,y,spread,p,configFileName);
}
BulletFactory::BulletFactory() {}
Bullet* BulletFactory::create(int x,int y,int spread,SpaceShip* p, string configFileName) {
	return new Bullet(x,y,spread,p,configFileName);
}
EBulletFactory::EBulletFactory() {}
EBullet* EBulletFactory::create(int x,int y,int spread,SpaceShip* p, string configFileName, GameObject* obj) {
	return new EBullet(x,y,spread,p,configFileName,obj);
}
ParticleFactory::ParticleFactory() {}
Particle* ParticleFactory::create(int x,int y,float vX,float vY,int spread,SpaceShip* p, string configFileName) {
	return new Particle(x,y,vX,vY,spread,p,configFileName);
}