#include "Includes.h"
#include "Help_Functions.h"

//------------------VECTOR OPERATIONS----------------------------
//Multiplicación de todas las coordenas de dos vectores, entra para calcular colision
double dotProduct(Vector3d& v1, Vector3d& v2){
	return (v1.x*v2.x+ v1.y*v2.y+ v1.z*v2.z);
}
//------------------VECTOR OPERATIONS----------------------------


//-----------------PARTICLE3D OPERATIONS-------------------------

//Crea un tipo de partículas segun el preset
void InitParticles(vector<Particle3d> & particulas, int size, Preset preset)
{
	switch (preset)
	{
		//Partículas con un ID
	case Preset_id:

		for (int i = 0; i<size; i++)
			particulas[i].id = i;

		break;

		//Partículas con una masa aleatoria
	case Preset_random_m:

		for (int i = 0; i<size; i++)
			particulas[i].mass = 2000.0*(rand() % 1000 + 0.001);
		break;

		//Partículas con un radio en función de la masa
	case Preset_radius:

		for (int i = 0; i<size; i++)
			particulas[i].radius = 0.75 + 0 * particulas[i].mass / (75 * pow(10.0, 10));
		break;

		//Partículas con una velocidad aleatoria
	case Preset_random_v:

		for (int i = 0; i<size; i++)
			particulas[i].velocity = Vector3d((rand() % 1000 - 500) / 100.0, (rand() % 1000 - 500) / 100.0, (rand() % 1000 - 500) / 100.0);
		break;

	default:
		break;
	}
}

void InitParticles(vector<Estrella> & particulas, int size, Preset preset)
{
	switch (preset)
	{
		//Partículas con un ID
	case Preset_id:

		for (int i = 0; i<size; i++)
			particulas[i].id = i;

		break;

		//Partículas con una masa aleatoria
	case Preset_random_m:

		for (int i = 0; i<size; i++)
			particulas[i].mass = 2000.0*(rand() % 1000 + 0.001);
		break;

		//Partículas con un radio en función de la masa
	case Preset_radius:

		for (int i = 0; i<size; i++)
			particulas[i].radius = 0.75 + 0 * particulas[i].mass / (75 * pow(10.0, 10));
		break;

		//Partículas con una velocidad aleatoria
	case Preset_random_v:

		for (int i = 0; i<size; i++)
			particulas[i].velocity = Vector3d((rand() % 1000 - 500) / 100.0, (rand() % 1000 - 500) / 100.0, (rand() % 1000 - 500) / 100.0);
		break;

	default:
		break;
	}
}

//Devuelvo un estado con los atributos de la partícula atributo
State ParticleToState(Particle3d& particula){

	State STATE;
	STATE.id = particula.id;
	STATE.mass = particula.mass;
	STATE.Position = particula.position;
	STATE.Velocity = particula.velocity;
	return STATE;
}

//Copia size estados en particulas
void CopyParticles(vector<State> & estados, vector<Particle3d> & particulas, int size){
	for (int i = 0; i<size; i++)
		estados[i] = ParticleToState(particulas[i]);
}

void CopyParticles(vector<State> & estados, vector<Estrella> & particulas, int size){
	for (int i = 0; i<size; i++)
		estados[i] = ParticleToState(particulas[i]);
}

//Copia size particulas en estados
void CopyStates(State* estados, Particle3d* particulas, int size){
	for (int i = 0; i<size; i++)
	{
		particulas[i].position = estados[i].Position;
		particulas[i].velocity = estados[i].Velocity;
		particulas[i].acceleration = estados[i].Acceleration;
	}
}

//-----------------PARTICLE3D OPERATIONS-------------------------

//-----------------DEBUG---------------------------

//Dibuja una linea desde una posición dada por coordenadas hasta otra dada por un vector
void DrawVector3d(Vector3d& v, double x, double y, double z)
{
	glBegin(GL_LINES);

	glVertex3f(x, y, z);
	glVertex3f(x + v.x, y + v.y, z + v.z);

	glEnd();
}

//-----------------DEBUG---------------------------

//-----------------UTILIDADES---------------------------

//Distancia entre dos puntos
double Distance3d(Vector3d p1, Vector3d p2){
	double d=sqrt( (p1.x-p2.x)*(p1.x-p2.x) +
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

GLfloat GetRandomNumber(GLfloat min, GLfloat max)
{	
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}

Vector3d GetGravity() { return Vector3d(0, 9.81f, 0); }