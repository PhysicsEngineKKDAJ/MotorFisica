#include "ParticleRainSystem.h"

ParticleRainSystem::ParticleRainSystem(Vector3d pos, GLfloat r)
{
	pos_ = pos;
	radius_ = r;
}


ParticleRainSystem::~ParticleRainSystem()
{
	std::list<Particle3d*>::iterator it = particles.begin();

	while (!particles.empty())
	{
		delete (*it);
		it = particles.erase(it);
	}

}
void ParticleRainSystem::update(GLfloat deltaTime)
{
	std::list<Particle3d*>::iterator it = particles.begin();

	while (!particles.empty() && it != particles.end())
	{
		(*it)->update(deltaTime);


		//Limpia las partículas muertas
		/*if ((*it)->getDestroy())
			it = particles.erase(it);
		else*/

			++it;
	}

	//Crea nuevas partículas para tener un flujo constante
	if (particles.size() < NumMaxParticles)
	{
		for (size_t i = 0; i < ParticlesPerTick; i++)
			particles.push_back(createParticle());
	}

}
/*
Constructora dinámica de partículas. Asigna una posicion inicial, un color y la velocidad-
Le da también un puntero al mundo en el que existe.
Y la devuelve.
*/
Particle3d* ParticleRainSystem::createParticle()
{
	//Aleatoriedad de la particula
	GLfloat ang = GetRandomNumber(0, 360);
	GLfloat random = GetRandomNumber(-radius_, radius_);
	GLfloat alt = GetRandomNumber(150, 250);
	int randomColor = GetRandomNumber(0, 5);

	Particle3d *p = new Particle3d(Vector3d(pos_.getX() + random * cos(ang), alt, pos_.getZ() + random * sin(ang)), 100, 300);

	switch (randomColor)
	{
	case 0:
		SetColor(COLOR::WHITE);			//BLANCO
		break;
	case 1:
		SetColor(COLOR::RED);			//ROJO
		break;
	case 2:
		SetColor(COLOR::BLUE);			//AZUL
		break;
	case 3:
		SetColor(COLOR::GREEN);			//VERDE
		break;
	case 4:
		SetColor(COLOR::YELLOW);		//AMARILLO
		break;

	default:
		break;
	}

	return p;
}

void ParticleRainSystem::dibuja() {
	if (!particles.empty())
	{
		std::list<Particle3d*>::iterator it = particles.begin();

		while (!particles.empty() && it != particles.end())
		{
			(*it)->dibuja();
			++it;
		}
	}

};