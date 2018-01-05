#include "TestParticles.h"


TestParticles::TestParticles()
{
	rainParticles1 = new ParticleRainSystem(Vector3d(100, 0, 0), 10);
	rainParticles2 = new ParticleRainSystem(Vector3d(0, 0, 100), 10);
}


TestParticles::~TestParticles()
{
	delete rainParticles1;
	delete rainParticles2;
}

void TestParticles::dibuja() {
	Escena::dibuja();

	GLfloat ticks = glutGet(GLUT_ELAPSED_TIME);
	rainParticles1->dibuja();
	rainParticles2->dibuja();

	if (lastTimeUpdate + frequency <= ticks)
	{
		rainParticles1->update(ticks);
		rainParticles2->update(ticks);
	}
}
