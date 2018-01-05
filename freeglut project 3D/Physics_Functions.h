#pragma once
#include "Includes.h"
#include "Help_Functions.h"

//CREO QUE HABRÍA QUE HACER TODO PHYSICS FUNCTIONS MAS BONITO

Vector3d accDirectionalGravity(State sip);	//g
Vector3d accUniversalGravitation(State sip); //G

void DirectionalGravity(int, Planetas3D*, short, State&, float);

void UniversalGravitation(int, Planetas3D*, short, State&, float, double);

void dImpenetrationResolution(int, int, Planetas3D*, State*, Vector3d ContactNormal);

void CollisionResolution(int p_c, Planetas3D *p, State *ps);

//-------------------------------------------
//-------------------------------------------

