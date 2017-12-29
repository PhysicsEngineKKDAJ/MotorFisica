#pragma once

#include "Includes.h"
#include "Primitives.h"


//METODOS DE AYUDA PARA VECTORES, PARTICULAS Y DEBUG


//---Vector Operations---
double dotProduct(Vector3d& v1, Vector3d& v2);

//---Particle3d---

//Crea un tipo de partículas según el preset
void InitParticles(vector<Particle3d> & particulas, int size, Preset preset);
void InitParticles(vector<Estrella> & particulas, int size, Preset preset);

State ParticleToState(Particle3d & particula);//Devuelvo un estado con los atributos de la partícula atributo

void CopyParticles(vector<State> & estados, vector<Particle3d> & particulas, int size);
void CopyParticles(vector<State> & estados, vector<Estrella> & particulas, int size);

void CopyStates(State*, Particle3d*, int);//Copia size particulas en estados


//---Drawing---
void DrawVector3d(Vector3d & v, double x, double y, double z);//Dibuja una linea desde una posición dada por coordenadas hasta otra dada por un vector
void SetColor(COLOR c);//Aplica un color del enum del atributo a la matriz de modelado

//---Debug---
double Distance3d(Vector3d p1, Vector3d p2);//Distancia entre dos puntos