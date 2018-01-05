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

	//Copia las planetas en los estados
	CopyPlanets(_estadoPlanetas, _planetas, _NumPlanetas);
	CopyPlanets(_estadoEstrellas, _estrellas, _NumEstrellas);
}


void EscenaOrbitas::dibuja() {
	//Mueve la escena a la posición dada por el raton
	Escena::dibuja();

	//Si pulsas la flecha derecha
	if (flechaDe)
		for (int i = 0; i < _NumPlanetas; i++)
			UniversalGravitation(i, _planetas.data(), _NumPlanetas, _estadoPlanetas[i], 1.0 / _deltat, _G);

	//Si pulsas la flecha izquierda
	if (flechaIz)
	for (int i = 0; i < _NumPlanetas; i++)//Calcula la gravedad direccional de cada una de las planetas
		DirectionalGravity(i, _planetas.data(), _NumPlanetas, _estadoPlanetas[i], 1.0 / _deltat);

	//Resuelve las colisiones que pudieran haber
	CollisionResolution(_NumPlanetas, _planetas.data(), _estadoPlanetas.data());

	//Copia el estado
	CopyStates(_estadoPlanetas.data(), _planetas.data(), _NumPlanetas);


	//Dibuja las planetas
	for (int i = 0; i<_NumPlanetas; i++)
		_planetas[i].dibuja();

	//A LAS ESTRELLAS Y AL DEBUG DEL RECORRIDO NO LES AFECTA LA LUZ
	glDisable(GL_LIGHTING);

	
	//---------------ESTRELLAS---------------
	SetColor(WHITE);
	glBegin(GL_POINTS);

	for (int i = 0; i<_NumEstrellas; i++)
		glVertex3f(_estrellas[i].position.x, _estrellas[i].position.y, _estrellas[i].position.z);
	
	glEnd();

	//---------------ESTRELLAS---------------

	//COMENTAR ESTO SI NO SE QUIERE EL RECORRIDO DE DEBUG
	//---------------RECORRIDO planetas---------------
	if (xActivate)
	{
		//Borra los puntos de trayectoria si hay demasiados
		for (int i = 0; i<_NumPlanetas; i++)
		{
			if (_posPlanetas[i].size()>2600000)
				_posPlanetas[i].erase(_posPlanetas[i].begin(), _posPlanetas[i].begin() + 1);
		}

		//Crea un nuevo punto de la trayectoria y lo añade al vector
		for (int i = 0; i < _NumPlanetas; i++)
			_posPlanetas[i].push_back(Vector3d(_planetas[i].position.x, _planetas[i].position.y, _planetas[i].position.z));

		//Dibuja todos los puntos de la trayectoria para cada particula
		glPointSize(1);
		SetColor(WHITE);
		glBegin(GL_POINTS);

		for (int i = 0; i < _NumPlanetas; i++)
		{
			for (int j = 1; j < _posPlanetas[i].size(); j++)
				glVertex3f(_posPlanetas[i][j].x, _posPlanetas[i][j].y, _posPlanetas[i][j].z);
		}
		glEnd();

	}
	
	glEnable(GL_LIGHTING);

	//---------------RECORRIDO planetas---------------

	//-------------VECTOR DEBUG----------------
	if (zActivate)
	{
		//Draw velocity and acceleration vectors
		SetColor(GREEN);
		for (int i = 0; i < _NumPlanetas; i++)
			DrawVector3d(_planetas[i].velocity*pow(1.89, _velocityDebug), _planetas[i].position.x, _planetas[i].position.y, _planetas[i].position.z);

		SetColor(RED);
		for (int i = 0; i < _NumPlanetas; i++)
			DrawVector3d(_planetas[i].acceleration*pow(3.1, _accelerationDebug), _planetas[i].position.x, _planetas[i].position.y, _planetas[i].position.z);
	}

	//-------------VECTOR DEBUG----------------

}

void EscenaOrbitas::createPlanetExample(){
	_NumPlanetas = 7;
	_NumEstrellas = 100;

	resizeVector();

	initParticlesID();

	_planetas[0].mass = 1000000000;
	_planetas[0].radius = 20;
	_planetas[0].position.set(0, 0, 0);

	_planetas[1].position.set(200, 0, 0);
	_planetas[1].velocity.set(0, 0, 150);
	_planetas[1].mass = 10000;
	_planetas[1].radius = 5;

	_planetas[2].position.set(0, 0, 100);
	_planetas[2].velocity.set(220, 0, 0);
	_planetas[2].mass = 100;
	_planetas[2].radius = 3;

	_planetas[3].position.set(0, 100, 100);
	_planetas[3].velocity.set(0, -300, -100);
	_planetas[3].mass = 1000;
	_planetas[3].radius = 1;

	_planetas[4].position.set(100, 100, 100);
	_planetas[4].velocity.set(0, -0, -100);
	_planetas[4].mass = 100000;
	_planetas[4].radius = 1;

	_planetas[5].position.set(0, 200, 000);
	_planetas[5].velocity.set(0, -100, 0);
	_planetas[5].mass = 1000;
	_planetas[5].radius = 4;

	_planetas[6].position.set(0, 100, 0);
	_planetas[6].velocity.set(0, 200, 0);
	_planetas[6].mass = 10000;
	_planetas[6].radius = 8;


}

void EscenaOrbitas::createSpiralExample(){
	_NumPlanetas = 200;
	_NumEstrellas = 1000;

	resizeVector();

	initParticlesID();

	//Crea todas las partículas con una masa aleatoria
	InitPlanets(_planetas, _NumPlanetas, Preset_random_m);

}

void EscenaOrbitas::createCollisionExample(){
	_NumPlanetas = 125;
	_NumEstrellas = 0;
	resizeVector();

	initParticlesID();

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				_planetas[i * 25 + j * 5 + k].position.set(i * 3, j * 3, k * 3);
				_planetas[i * 25 + j * 5 + k].velocity.set(0, 0, 0);
				_planetas[i * 25 + j * 5 + k].mass = 5000 + i * 25 + j * 5 + k;
				_planetas[i * 25 + j * 5 + k].radius = 1;
			}
		}

	}

	_planetas[0].position.set(5, 5, 100);
	_planetas[0].velocity.set(0, 0, -1000);
	_planetas[0].mass = 1000000;
	_planetas[0].radius = 10;


}

void EscenaOrbitas::initParticlesID()
{
	//Da un ID a todas las planetas
	InitPlanets(_planetas, _NumPlanetas, Preset_id);

	//Da un ID a todas las estrellas
	InitPlanets(_estrellas, _NumEstrellas, Preset_id);
}

void EscenaOrbitas::resizeVector()
{
	_estadoPlanetas.resize(_NumPlanetas);
	_planetas.resize(_NumPlanetas);
	_posPlanetas.resize(_NumPlanetas);
	_estrellas.resize(_NumEstrellas);
	_estadoEstrellas.resize(_NumEstrellas);
}