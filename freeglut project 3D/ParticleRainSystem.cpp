#include "ParticleRainSystem.h"

ParticleRainSystem::ParticleRainSystem(Vector3d pos, GLfloat r)
{
	pos_ = pos;
	radius_ = r;
}


ParticleRainSystem::~ParticleRainSystem()
{
	std::list<Particle3D*>::iterator it = particles.begin();

	while (!particles.empty())
	{
		delete (*it);
		it = particles.erase(it);
	}

}
void ParticleRainSystem::update(GLfloat deltaTime)
{
	std::list<Particle3D*>::iterator it = particles.begin();

	while (!particles.empty() && it != particles.end())
	{
		(*it)->update(deltaTime);


		//Limpia las partículas muertas
		if ((*it)->getDestroy())
			it = particles.erase(it);
		else

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
Particle3D* ParticleRainSystem::createParticle()
{
	//Aleatoriedad de la particula
	GLfloat ang = GetRandomNumber(0, 360);
	GLfloat random = GetRandomNumber(-radius_, radius_);
	GLfloat alt = GetRandomNumber(150, 250);
	int randomColor = GetRandomNumber(0, 5);

	Particle3D *p = new Particle3D(Vector3d(pos_.getX() + random * cos(ang), alt, pos_.getZ() + random * sin(ang)), 100, 300);

	switch (randomColor)
	{
	case 0:
		p->setColor(Color{ 1, 1, 1, 1 });			//BLANCO
		break;
	case 1:
		p->setColor(Color{ 1, 0, 0, 1 });			//ROJO
		break;
	case 2:
		p->setColor(Color{ 0, 0, 1, 1 });			//AZUL
		break;
	case 3:
		p->setColor(Color{ 0, 1, 0, 1 });			//VERDE
		break;
	case 4:
		p->setColor(Color{ 1, 0.4, 1, 1 });			//ROSA
		break;

	default:
		break;
	}

	return p;
}

void ParticleRainSystem::dibuja() {
	if (!particles.empty())
	{
		std::list<Particle3D*>::iterator it = particles.begin();

		while (!particles.empty() && it != particles.end())
		{
			(*it)->dibuja();
			++it;
		}
	}

};
