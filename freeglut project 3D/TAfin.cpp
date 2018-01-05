#include "TAfin.h"

TAfin::TAfin()
{
	m = new GLfloat[16];
	for (int i = 0; i < 16; i++)
		m[i] = 0;

	m[0] = m[5] = m[10] = m[15] = 1;//LA IDENTIDAD
	//...
}

void TAfin::reset(){
	for (int i = 0; i < 16; i++) m[i] = 0;
	m[0] = m[5] = m[10] = m[15] = 1;
}

void TAfin::traslada(Vector3d* v) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//post-multiplica la matriz de modelado-vista por la matriz de traslación asociada al vector determinado por los tres parámetros.
	glTranslatef(v->getX(), v->getY(), v->getZ());

	GLfloat m1[16];
	//Dejar la matriz actual de modelado-vista en m1
	//Los 16 datos están enumerados por columnas //esta hecho
	glGetFloatv(GL_MODELVIEW_MATRIX, m1);
	glPopMatrix();
	postMultiplica(m1);
}

//En sentido antihorario
void TAfin::rota(GLfloat a, GLfloat x, GLfloat y, GLfloat z) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(a, x, y, z);
	GLfloat m1[16];
	//Dejar la matriz actual de modelado-vista en m1
	//Los 16 datos están enumerados por columnas
	glGetFloatv(GL_MODELVIEW_MATRIX, m1);
	glPopMatrix();
	postMultiplica(m1);
}

void TAfin::escala(Vector3d* v) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(v->getX(), v->getY(), v->getZ());
	GLfloat m1[16];
	//Dejar la matriz actual de modelado-vista en m1
	//Los 16 datos están enumerados por columnas
	glGetFloatv(GL_MODELVIEW_MATRIX, m1);
	glPopMatrix();
	postMultiplica(m1);
}

void TAfin::postMultiplica(GLfloat* m1) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//Cargar m como matriz actual de modelado-vista
	glLoadMatrixf(m);
	//Post-multiplicar por m1
	glMultMatrixf(m1);
	//Dejar el resultado en m
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();
}