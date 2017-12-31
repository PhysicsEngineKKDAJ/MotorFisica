#include"Primitives.h"
#include "Help_Functions.h"



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

Particle3d::Particle3d(Vector3d p, GLfloat maxVida, GLfloat minVida){
	position = p;
	velocity = Vector3d(0, 0, 0);
	acceleration = GetGravity();
	seconds_ = glutGet(GLUT_ELAPSED_TIME);
	//life_ = lifeAct_ = GetRandomNumber(minVida, maxVida);
	//size_ = 1;
}
void Particle3d::dibuja()
{
	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	SetColor(G75);
	glutSolidSphere(radius, 40, 40);

	glPopMatrix();

}

void Particle3d::update(GLfloat dt) {
	dt -= seconds_;
	dt /= 10000;

	Vector3d aceleration = acceleration;
	aceleration.operator*=(dt);

	velocity.operator+=(aceleration);

	Vector3d Velocidad = velocity;
	Velocidad.operator*=(dt);

	position.operator+=(Velocidad);

	/*lifeAct_--;
	if (position.getY() <= DeadTriggerDown || position.getY() >= DeadTriggerUp || lifeAct_ <= 0)
		setDestroy(true);*/


}
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
