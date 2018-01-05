#include "Planetas3D.h"
#include "Help_Functions.h"



void Planetas3D::dibuja()
{
	glPushMatrix();

	glMultMatrixf(this->mT->getM());
	//glTranslatef(position.x, position.y, position.z);
	SetColor(G75);
	glutSolidSphere(radius, 40, 40);

	glPopMatrix();

}
