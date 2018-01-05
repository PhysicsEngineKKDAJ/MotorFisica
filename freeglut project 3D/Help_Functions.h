#pragma once

#include "Includes.h"
#include "Planetas3D.h"

//METODOS DE AYUDA PARA VECTORES, planetas Y DEBUG


//---Vector Operations---
double dotProduct(Vector3d& v1, Vector3d& v2);

//---Particle3d---

//Crea un tipo de partículas según el preset
void InitPlanets(vector<Planetas3D> & planetas, int size, Preset preset);
void InitPlanets(vector<Estrella> & planetas, int size, Preset preset);

State PlanetToState(Planetas3D & planeta);//Devuelvo un estado con los atributos de la partícula atributo

void CopyPlanets(vector<State> & estados, vector<Planetas3D> & planetas, int size);
void CopyPlanets(vector<State> & estados, vector<Estrella> & planetas, int size);

void CopyStates(State*, Planetas3D*, int);//Copia size planetas en estados

//Estos dos métodos sustituyen a los que había en la clase World antigua
GLfloat GetRandomNumber(GLfloat min, GLfloat max);
Vector3d GetGravity();

//---Drawing---
void DrawVector3d(Vector3d & v, double x, double y, double z);//Dibuja una linea desde una posición dada por coordenadas hasta otra dada por un vector
void SetColor(COLOR c);//Aplica un color del enum del atributo a la matriz de modelado

//---Debug---
double Distance3d(Vector3d p1, Vector3d p2);//Distancia entre dos puntos