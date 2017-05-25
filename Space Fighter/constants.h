#ifndef CONSTANTS_H
#define CONSTANTS_H

const float PI = 3.141592654;
const float TURN = (2*PI)/72;
const float DELTA_T = 0.05; //used until it can be dynamic based on framerate!
const float E = 0.1;
const float SPEED = 75;
const float E_SPD =	50;
const float SHIP_MASS = 25;
const float AST_MASS = 3000;
const float BULLET_MASS = 1;
const float G = 6.67384e-5;
const int G_RANGE = 1000;
const int FPR_MAX = 5;
const int BULLET_LIFE = 60;
const int BULLET_CHARGE = 5;
const float BULLET_SPD = .5;
const float THRUST_SPD = (SPEED/SHIP_MASS)*DELTA_T;
const float TO_DEGREES = 180/PI; //to convert radian-based directional system into degree-based system for Directional Sound

//Ship AI & states
const float MAX_VEL = 20;
const int SIGHT_RANGE = 500;
const int GUARD_RADIUS = 1000;
const int IDLE = 0;
const int ATTACK = 1;

#endif