#pragma once

#include "stdafx.h"
#include "Primitives.h"

/*
METODOS DE AYUDA PARA VECTORES, PARTICULAS Y DEBUG
*/

//---Vector Operations---
real dotProduct(Vector3d& v1, Vector3d& v2);

//---Particle3d---
void InitP3d(Particle3d* P, int size, P3d_preset); //Crea un tipo de partículas según el preset
State P3dToState(Particle3d&p);//Devuelvo un estado con los atributos de la partícula atributo
void CopyParticles(State*, Particle3d*, int);//Copia size estados en particulas
void CopyStates(State*, Particle3d*, int);//Copia size particulas en estados


//---Drawing---
void DrawVector3d(Vector3d& v, real x, real y, real z);//Dibuja una linea desde una posición dada por coordenadas hasta otra dada por un vector
void DrawParticle3d(Particle3d&);//Dibuja los puntos blancos de debug que muestran la trayectoria

//---Debug---
real Distance3d(Vector3d p1, Vector3d p2);//Distancia entre dos puntos
void SetColor(COLOR);//Aplica un color del enum del atributo a la matriz de modelado