#pragma once
#include "Includes.h"
#include "Help_Functions.h"
#include "Global.h"



Vector3d accDirectionalGravity(State sip);	//g
Vector3d accUniversalGravitation(State sip); //G


void DirectionalGravity(int, Particle3d*, short, State&, float);
void DirectionalGravity_simple(int, Particle3d*, short, State&, float);

void UniversalGravitation(int, Particle3d*, short, State&, float, double);
void UniversalGravitation_simple(int, Particle3d*, short, State&, float, double);

void dImpenetrationResolution(int, int, Particle3d*, State*);
void CollisionResolution2(int p_c, Particle3d *p, State *ps);

void CollisionResolution(int p_c, Particle3d *p, State *ps);
void CollisionResolution_Ground(int p_c, Particle3d *p, State *ps);	//EN DESUSO PARA ESTA SIMULACIÓN