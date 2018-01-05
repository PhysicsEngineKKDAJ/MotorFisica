#pragma once

#include "Escena.h"
#include "ParticleRainSystem.h"

class TestParticles : public Escena
{
public:
	TestParticles();
	virtual ~TestParticles();

	virtual void dibuja();

private:
	GLfloat lastTimeUpdate, frequency;

	ParticleRainSystem * rainParticles1;
	ParticleRainSystem * rainParticles2;
	
};

