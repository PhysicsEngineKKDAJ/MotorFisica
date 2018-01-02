#pragma once
#include "Escena.h"


	//const short NumParticulas = 125;

class EscenaOrbitas : public Escena
{
public:
	EscenaOrbitas();
	virtual ~EscenaOrbitas() {};

	virtual void dibuja();
	virtual void update() {}; //VACIO DE MOMENTO

	void cambiaDer() { derechaOrb = !derechaOrb; }
	void cambiaIzq() { izquierdaOrb = !izquierdaOrb; }

private: 

	//Valores necesarios para el dibuja
	const double deltat = 1000;			 //Constante para dividir y que no se vaya de madre
	const double G = 6.67384*0.00080;    //Constante que usas en la gravitacion universal

	//Controlan el tamaño de las lineas verdes y rojas de debug
	//Cuánto más grande sea, más pequeñas son las lineas
	float velocityDebug = -7;
	float accelerationDebug = -7;

	

	//Booleanos que determinan el comportamiento del dibuja
	bool derechaOrb, izquierdaOrb;

	//VARIABLES GLOBALES DE LA CLASE
	vector <Particle3d> particulas;
	vector<State> estadoParticulas;

	//Este vector guarda todas las posiciones antiguas de cada particula

	//vector<vector<Vector3d>> posParticulas(int);
};

