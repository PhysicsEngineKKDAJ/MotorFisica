#pragma once
#include "Escena.h"

//const short NumParticulas = 125;

class EscenaOrbitas : public Escena
{
public:
	EscenaOrbitas();
	virtual ~EscenaOrbitas() {};

	virtual void dibuja();

private:
	
	//Valores necesarios para el dibuja
	const double _deltat = 1000;			 //Constante para dividir y que no se vaya de madre
	const double _G = 6.67384*0.00080;    //Constante que usas en la gravitacion universal

	//Controlan el tamaño de las lineas verdes y rojas de debug
	//Cuánto más grande sea, más pequeñas son las lineas
	float _velocityDebug = -7;
	float _accelerationDebug = -7;

	const short _NumParticulas = 125;

	//VARIABLES GLOBALES DE LA CLASE
	vector<State> _estadoParticulas;
	vector <Particle3d> _particulas;

	//Este vector guarda todas las posiciones antiguas de cada particula

	//vector<vector<Vector3d>> posParticulas(int);
};

