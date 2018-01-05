#include "EscenaOrbitas.h"
#include "Physics_Functions.h"

EscenaOrbitas::EscenaOrbitas() :_estadoParticulas(_NumParticulas), _particulas(_NumParticulas)
{
	//Da un ID a todas las particulas
	InitParticles(_particulas, _NumParticulas, Preset_id);

	//Da un ID a todas las estrellas
	//InitParticles(estrellas, NumEstrellas, Preset_id);

	//Crea todas las partículas con una masa aleatoria
	InitParticles(_particulas, _NumParticulas, Preset_random_m);


	//EL CHOQUECITO
	/*
	for (int i = 0; i < 5; i++)
	{
	for (int j = 0; j < 5; j++)
	{
	for (int k = 0; k < 5; k++)
	{

	particulas[i * 25 + j*5 + k].position.Set(i * 3, j * 3,k*3);
	particulas[i * 25 + j * 5 + k].velocity.Set(0, 0, 0);
	particulas[i * 25 + j * 5 + k].mass = 5000 + i * 25 + j*5 + k;
	particulas[i * 25 + j * 5 + k].radius = 1;
	}
	}

	}

	particulas[0].position.Set(5, 5, 100);
	particulas[0].velocity.Set(0, 0, -1000);
	particulas[0].mass = 1000000;
	particulas[0].radius = 10;
	*/
	/*PLANETAS ORBITANDO
	//test:
	particulas[0].mass=1000000000;
	particulas[0].radius = 20;
	particulas[0].position.Set(0, 0, 0);

	particulas[1].position.Set(200, 0, 0);
	particulas[1].velocity.Set(0, 0, 150);
	particulas[1].mass = 10000;
	particulas[1].radius = 5;

	particulas[2].position.Set(0, 0, 100);
	particulas[2].velocity.Set(220, 0, 0);
	particulas[2].mass = 100;
	particulas[2].radius= 3;

	particulas[3].position.Set(0, 100, 100);
	particulas[3].velocity.Set(0, -300, -100);
	particulas[3].mass = 1000;
	particulas[3].radius = 1;

	particulas[4].position.Set(100, 100, 100);
	particulas[4].velocity.Set(0, -0, -100);
	particulas[4].mass = 100000;
	particulas[4].radius = 1;
	*/

	/*
	particulas[5].position.Set(0, 200, 000);
	particulas[5].velocity.Set(0, -100, 0);
	particulas[5].mass = 1000;
	particulas[5].radius = 4;


	particulas[6].position.Set(0, 100, 0);
	particulas[6].velocity.Set(0, 200, 0);
	particulas[6].mass = 10000;
	particulas[6].radius = 8;
	*/

	CopyParticles(_estadoParticulas, _particulas, _NumParticulas);
	//CopyParticles(estadoEstrellas, estrellas, NumEstrellas);
}


void EscenaOrbitas::dibuja() {
	Escena::dibuja();
	//Si pulsas la flecha derecha
	if (flechaDe)
		for (int i = 0; i < _NumParticulas; i++)
			UniversalGravitation(i, _particulas.data(), _NumParticulas, _estadoParticulas[i], 1.0 / _deltat, _G);

	//Si pulsas la flecha izquierda
	if (flechaIz)
		for (int i = 0; i < _NumParticulas; i++)//Calcula la gravedad direccional de cada una de las particulas
			DirectionalGravity(i, _particulas.data(), _NumParticulas, _estadoParticulas[i], 1.0 / _deltat);

	//Resuelve las colisiones que pudieran haber
	CollisionResolution(_NumParticulas, _particulas.data(), _estadoParticulas.data());

	//Copia el estado
	CopyStates(_estadoParticulas.data(), _particulas.data(), _NumParticulas);


	//Dibuja las particulas
	for (int i = 0; i<_NumParticulas; i++)
		_particulas[i].dibuja();

	//A LAS ESTRELLAS Y AL DEBUG DEL RECORRIDO NO LES AFECTA LA LUZ
	glDisable(GL_LIGHTING);

	/*
	//---------------ESTRELLAS---------------

	for (int i = 0; i<NumEstrellas; i++)
	{
		SetColor(WHITE);
		glBegin(GL_POINTS);

		glVertex3f(estrellas[i].position.x, estrellas[i].position.y, estrellas[i].position.z);
		glEnd();
	}
	*/

	//---------------ESTRELLAS---------------

	//COMENTAR ESTO SI NO SE QUIERE EL RECORRIDO DE DEBUG
	//---------------RECORRIDO PARTICULAS---------------
	/*
	//Borra los puntos de trayectoria si hay demasiados
	for (int i = 0; i<NumParticulas; i++)
	{
		if (posParticulas[i].size()>2600000)
			posParticulas[i].erase(posParticulas[i].begin(), posParticulas[i].begin() + 1);
	}

	//Crea un nuevo punto de la trayectoria y lo añade al vector
	for (int i = 0; i<NumParticulas; i++)
		posParticulas[i].push_back(Vector3d(particulas[i].position.x, particulas[i].position.y, particulas[i].position.z));

	//Dibuja todos los puntos de la trayectoria para cada particula
	for (int i = 0; i<NumParticulas; i++)
	{
		glPointSize(1);
		SetColor(WHITE);
		glBegin(GL_POINTS);

		for (int j = 1; j<posParticulas[i].size(); j++)
			glVertex3f(posParticulas[i][j].x, posParticulas[i][j].y, posParticulas[i][j].z);

		glEnd();
	}
	*/
	glEnable(GL_LIGHTING);

	//---------------RECORRIDO PARTICULAS---------------

	//COMENTAR ESTO SI NO SE QUIEREN LINEAS DE DEBUG
	//-------------VECTOR DEBUG----------------

	//Draw velocity and acceleration vectors
	SetColor(GREEN);
	for (int i = 0; i<_NumParticulas; i++)
		DrawVector3d(_particulas[i].velocity*pow(1.89, _velocityDebug), _particulas[i].position.x, _particulas[i].position.y, _particulas[i].position.z);

	SetColor(RED);
	for (int i = 0; i<NumParticulas; i++)
		DrawVector3d(_particulas[i].acceleration*pow(3.1, _accelerationDebug), _particulas[i].position.x, _particulas[i].position.y, _particulas[i].position.z);


	//-------------VECTOR DEBUG----------------

}
