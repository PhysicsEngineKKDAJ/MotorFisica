#include"Primitives.h"
#include "Help_Functions.h"


void Particle3d::dibuja()
{
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	SetColor(G75);
	glutSolidSphere(radius, 40, 40);

	glPopMatrix();
}

