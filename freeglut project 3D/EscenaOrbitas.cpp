#include "EscenaOrbitas.h"
#include "Physics_Functions.h"

EscenaOrbitas::EscenaOrbitas(TEscenas tipoEscena)
{
	switch (tipoEscena)
	{
	case TEscenas::EEspiral:
		createSpiralExample();
		break;
	case TEscenas::EPlanetas:
		createPlanetExample();
		break;	

	case TEscenas::EColision:
		createCollisionExample();
		break;

	}

	//Copia las particulas en los estados
	CopyParticles(_estadoParticulas, _particulas, _NumParticulas);
	CopyParticles(_estadoEstrellas, _estrellas, _NumEstrellas);
}


void EscenaOrbitas::dibuja() {
	//Mueve la escena a la posición dada por el raton
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

	
	//---------------ESTRELLAS---------------

	for (int i = 0; i<_NumEstrellas; i++)
	{
		SetColor(WHITE);
		glBegin(GL_POINTS);

		glVertex3f(_estrellas[i].position.x, _estrellas[i].position.y, _estrellas[i].position.z);
		glEnd();
	}
	

	//---------------ESTRELLAS---------------

	//COMENTAR ESTO SI NO SE QUIERE EL RECORRIDO DE DEBUG
	//---------------RECORRIDO PARTICULAS---------------
	if (xActivate)
	{
		//Borra los puntos de trayectoria si hay demasiados
		for (int i = 0; i<_NumParticulas; i++)
		{
			if (_posParticulas[i].size()>2600000)
				_posParticulas[i].erase(_posParticulas[i].begin(), _posParticulas[i].begin() + 1);
		}

		//Crea un nuevo punto de la trayectoria y lo añade al vector
		for (int i = 0; i < _NumParticulas; i++)
			_posParticulas[i].push_back(Vector3d(_particulas[i].position.x, _particulas[i].position.y, _particulas[i].position.z));

		//Dibuja todos los puntos de la trayectoria para cada particula
		for (int i = 0; i < _NumParticulas; i++)
		{
			glPointSize(1);
			SetColor(WHITE);
			glBegin(GL_POINTS);

			for (int j = 1; j < _posParticulas[i].size(); j++)
				glVertex3f(_posParticulas[i][j].x, _posParticulas[i][j].y, _posParticulas[i][j].z);

			glEnd();
		}
	}
	
	glEnable(GL_LIGHTING);

	//---------------RECORRIDO PARTICULAS---------------

	//-------------VECTOR DEBUG----------------
	if (zActivate)
	{
		//Draw velocity and acceleration vectors
		SetColor(GREEN);
		for (int i = 0; i < _NumParticulas; i++)
			DrawVector3d(_particulas[i].velocity*pow(1.89, _velocityDebug), _particulas[i].position.x, _particulas[i].position.y, _particulas[i].position.z);

		SetColor(RED);
		for (int i = 0; i < _NumParticulas; i++)
			DrawVector3d(_particulas[i].acceleration*pow(3.1, _accelerationDebug), _particulas[i].position.x, _particulas[i].position.y, _particulas[i].position.z);
	}

	//-------------VECTOR DEBUG----------------

}

void EscenaOrbitas::createPlanetExample(){
	_NumParticulas = 7;
	_NumEstrellas = 100;

	resizeVector();

	initParticlesID();

	_particulas[0].mass = 1000000000;
	_particulas[0].radius = 20;
	_particulas[0].position.set(0, 0, 0);

	_particulas[1].position.set(200, 0, 0);
	_particulas[1].velocity.set(0, 0, 150);
	_particulas[1].mass = 10000;
	_particulas[1].radius = 5;

	_particulas[2].position.set(0, 0, 100);
	_particulas[2].velocity.set(220, 0, 0);
	_particulas[2].mass = 100;
	_particulas[2].radius = 3;

	_particulas[3].position.set(0, 100, 100);
	_particulas[3].velocity.set(0, -300, -100);
	_particulas[3].mass = 1000;
	_particulas[3].radius = 1;

	_particulas[4].position.set(100, 100, 100);
	_particulas[4].velocity.set(0, -0, -100);
	_particulas[4].mass = 100000;
	_particulas[4].radius = 1;

	_particulas[5].position.set(0, 200, 000);
	_particulas[5].velocity.set(0, -100, 0);
	_particulas[5].mass = 1000;
	_particulas[5].radius = 4;

	_particulas[6].position.set(0, 100, 0);
	_particulas[6].velocity.set(0, 200, 0);
	_particulas[6].mass = 10000;
	_particulas[6].radius = 8;


}

void EscenaOrbitas::createSpiralExample(){
	_NumParticulas = 100;
	_NumEstrellas = 1000;

	resizeVector();

	initParticlesID();

	//Crea todas las partículas con una masa aleatoria
	InitParticles(_particulas, _NumParticulas, Preset_random_m);

}

void EscenaOrbitas::createCollisionExample(){
	_NumParticulas = 125;
	_NumEstrellas = 0;
	resizeVector();

	initParticlesID();

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				_particulas[i * 25 + j * 5 + k].position.set(i * 3, j * 3, k * 3);
				_particulas[i * 25 + j * 5 + k].velocity.set(0, 0, 0);
				_particulas[i * 25 + j * 5 + k].mass = 5000 + i * 25 + j * 5 + k;
				_particulas[i * 25 + j * 5 + k].radius = 1;
			}
		}

	}

	_particulas[0].position.set(5, 5, 100);
	_particulas[0].velocity.set(0, 0, -1000);
	_particulas[0].mass = 1000000;
	_particulas[0].radius = 10;


}

void EscenaOrbitas::initParticlesID()
{
	//Da un ID a todas las particulas
	InitParticles(_particulas, _NumParticulas, Preset_id);

	//Da un ID a todas las estrellas
	InitParticles(_estrellas, _NumEstrellas, Preset_id);
}

void EscenaOrbitas::resizeVector()
{
	_estadoParticulas.resize(_NumParticulas);
	_particulas.resize(_NumParticulas);
	_posParticulas.resize(_NumParticulas);
	_estrellas.resize(_NumEstrellas);
	_estadoEstrellas.resize(_NumEstrellas);
}