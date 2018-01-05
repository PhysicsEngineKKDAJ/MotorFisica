#include "Particle3D.h"
#include "Help_Functions.h"

Particle3D::Particle3D(Vector3d p, GLfloat maxVida, GLfloat minVida) :Objeto3D()
{
	pos_ = p;
	vel_ = Vector3d(0, 0, 0);
	acc_ = GetGravity();;
	seconds_ = glutGet(GLUT_ELAPSED_TIME);
	life_ = lifeAct_ = GetRandomNumber(minVida, maxVida);
	size_ = 1;
}


Particle3D::~Particle3D()
{
}

void Particle3D::dibuja() {

	glPushMatrix();

	glTranslated(pos_.getX(), pos_.getY(), pos_.getZ());
	glColor4f(color_.r, color_.g, color_.b, color_.a);

	glutSolidSphere(size_, 5, 5);

	glPopMatrix();

}


void Particle3D::update(GLfloat dt) {
	dt -= seconds_;
	dt /= 10000;

	Vector3d aceleration = acc_;
	aceleration.operator*=(dt);

	vel_.operator+=(aceleration);

	Vector3d Velocidad = vel_;
	Velocidad.operator*=(dt);

	pos_.operator+=(Velocidad);


	lifeAct_--;
	if (pos_.getY() <= DeadTriggerDown || pos_.getY() >= DeadTriggerUp || lifeAct_ <= 0)
		setDestroy(true);


}