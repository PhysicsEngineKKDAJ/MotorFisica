#include "stdafx.h"
#include "Help_Functions.h"

//------------------VECTOR OPERATIONS----------------------------
//Multiplicación de todas las coordenas de dos vectores, entra para calcular colision
real dotProduct(Vector3d& v1, Vector3d& v2){
	return (v1.x*v2.x+ v1.y*v2.y+ v1.z*v2.z);
}

//------------------VECTOR OPERATIONS----------------------------


//-----------------PARTICLE3D OPERATIONS-------------------------

//Crea un tipo de partículas segun el preset
void InitP3d(Particle3d* P, int size, P3d_preset p3d)
{
	switch (p3d)
	{
		//Particulas de radio 0
	case p3d_point_particles:

		for (int i = 0; i<size; i++)
			P[i].radius = 0.0;

		break;

		//Partículas con un ID
	case p3d_id:

		for (int i = 0; i<size; i++)
			P[i].id = i;

		break;

		//Partículas con una masa aleatoria
	case p3d_random_m:

		for (int i = 0; i<size; i++)
			P[i].mass = 2000.0*(rand() % 1000 + 0.001);
		break;

		//Partículas con un radio en función de la masa
	case p3d_radius:

		for (int i = 0; i<size; i++)
			P[i].radius = 0.75 + 0 * P[i].mass / (75 * pow(10.0, 10));
		break;

		//Partículas con una velocidad aleatoria
	case p3d_random_v:

		for (int i = 0; i<size; i++)
			P[i].velocity = Vector3d((rand() % 1000 - 500) / 100.0, (rand() % 1000 - 500) / 100.0, (rand() % 1000 - 500) / 100.0);
		break;

	default:
		break;
	}
}

//Devuelvo un estado con los atributos de la partícula atributo
State P3dToState(Particle3d&p){

	State STATE;
	STATE.i = p.id;
	STATE.mass = p.mass;
	STATE.Position = p.position;
	STATE.Velocity = p.velocity;
	return STATE;
}

//Copia size estados en particulas
void CopyParticles(State* S, Particle3d* P, int size){
	for (int i = 0; i<size; i++)
		S[i] = P3dToState(P[i]);
}

//Copia size particulas en estados
void CopyStates(State* S, Particle3d* P, int size){
	for (int i = 0; i<size; i++)
	{
		P[i].position = S[i].Position;
		P[i].velocity = S[i].Velocity;
		P[i].acceleration = S[i].Acceleration;
		//P[i]=StateToP3d(S[i]);

	}
}

//-----------------PARTICLE3D OPERATIONS-------------------------

//-----------------DEBUG---------------------------
//Dibuja una linea desde una posición dada por coordenadas hasta otra dada por un vector
void DrawVector3d(Vector3d& v, real x, real y, real z)
{
	//ESTAS DOS COSAS NO LAS USA
	Vector3d tempv = v;
	tempv.Normalize();//normalize to get direction for drawing arrow head

	glBegin(GL_LINES);
	glVertex3f(x, y, z);
	glVertex3f(x + v.x, y + v.y, z + v.z);

	//glVertex3f(x+v.x, y+v.y, z+v.z);
	//glVertex3f(x+v.x-tempv.x*0.1*cos(3.14/6), y+v.y+0.06*sin(3.14/6), z+v.z);
	glEnd();
}

//Dibuja los puntos blancos de debug que muestran la trayectoria
void DrawParticle3d(Particle3d &p)
{
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(p.position.x, p.position.y, p.position.z);
	SetColor(G75);

	glutSolidSphere(p.radius, 40, 40);
	//cout<<"drawing at x:"<<p.position.x<<", y: "<<p.position.y<<", z:"<< p.position.z<<endl;;
	glPopMatrix();
}


//-----------------DEBUG---------------------------

//-----------------UTILIDADES---------------------------

//Distancia entre dos puntos
real Distance3d(Vector3d p1, Vector3d p2){
	real d=sqrt( (p1.x-p2.x)*(p1.x-p2.x) +
				 (p1.y-p2.y)*(p1.y-p2.y) +
				 (p1.z-p2.z)*(p1.z-p2.z)
			   );
	return d;
}

//Aplica un color del enum del atributo a la matriz de modelado
void SetColor(COLOR color){

	switch(color)
	{
		case BLACK:
		glColor3f(0.0, 0.0, 0.0);
		break;

		case G75:
		glColor3f(0.75, 0.75, 0.75);
		break;

		case G50:
		glColor3f(0.5, 0.5, 0.5);
		break;

		case G25:
		glColor3f(0.25, 0.25, 0.25);
		break;

		case WHITE:
		glColor3f(1.0, 1.0, 1.0);
		break;

		case RED:
		glColor3f(1.0, 0.0, 0.0);
		break;

		case GREEN:
		glColor3f(0.0, 1.0, 0.0);
		break;

		case BLUE:
		glColor3f(0.0, 0.0, 1.0);
		break;

		case YELLOW:
		glColor3f(1.0, 1.0, 0.0);
		break;

		case MAGENTA:
		glColor3f(1.0, 0.0, 1.0);
		break;
	default:
		break;
	}
}
//-----------------UTILIDADES---------------------------
