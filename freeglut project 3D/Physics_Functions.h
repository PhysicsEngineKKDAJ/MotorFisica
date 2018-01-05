#pragma once
#include "Includes.h"
#include "Help_Functions.h"

//CREO QUE HABRÍA QUE HACER TODO PHYSICS FUNCTIONS MAS BONITO

Vector3d accDirectionalGravity(State sip);	//g
Vector3d accUniversalGravitation(State sip); //G

void DirectionalGravity(int, Particle3d*, short, State&, float);

void UniversalGravitation(int, Particle3d*, short, State&, float, double);

void dImpenetrationResolution(int, int, Particle3d*, State*, Vector3d ContactNormal);

void CollisionResolution(int p_c, Particle3d *p, State *ps);

//-------------------------------------------
//-------------------------------------------

