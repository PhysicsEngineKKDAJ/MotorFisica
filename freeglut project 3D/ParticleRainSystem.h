#ifndef _ParticleRainSystem_H_
#define _ParticleRainSystem_H_
/*
Sistema de part�culas que simula la lluvia
mediante un flujo constante de part�culas generadas
en una altura determinada y que caen,
muriendo a los pocos segundos.
*/

//#include "Objeto3DFisico.h"

#include "Particle3D.h"
#include "Help_Functions.h"
#include <list>

class ParticleRainSystem : public Vector3d
{
public:
	ParticleRainSystem(Vector3d pos, GLfloat r);
	virtual ~ParticleRainSystem();

	virtual void dibuja();							// Viene de Objeto3D. Dibuja el vector.
	virtual void update(GLfloat dt);					//Update de las particulas: Determina su pos y vida.

	Particle3D *createParticle();					//Creadora din�mica de part�culas

private:
	std::list<Particle3D*> particles;			//lista de part�culas

	GLfloat radius_; //Radio de las part�culas
	const int NumMaxParticles = 400;
	const int ParticlesPerTick = 10;
	Vector3d pos_;
};

#endif
