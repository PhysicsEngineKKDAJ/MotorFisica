#include "Vector3D.h"

//Producto escalar
GLfloat Vector3d::productoEscalar(Vector3d* vector) {
	return x*vector->getX() +
		y*vector->getY() +
		z*vector->getZ();
}
//Producto Vectorial
Vector3d* Vector3d::productoVectorial(Vector3d* v) {
	GLfloat resx = 0;
	GLfloat resy = 0;
	GLfloat resz = 0;
	resx = this->y*v->z - v->y*this->z;
	resy = this->z*v->x - v->z*this->x;
	resz = this->x*v->y - v->x*this->y;
	return new Vector3d(resx, resy, resz);
}

/*
void PuntoVector3D::normalizar() {
GLfloat modulo2=productoEscalar(this);
if (modulo2>0) {
x=x/sqrt(modulo2);
y=y/sqrt(modulo2);
z=z/sqrt(modulo2);
}
}

*/