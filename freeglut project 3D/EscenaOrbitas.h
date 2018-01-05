#pragma once
#include "Escena.h"

//const short NumParticulas = 125;

class EscenaOrbitas : public Escena
{
public:
	EscenaOrbitas(TEscenas tipoEscena);
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

	short _NumParticulas = 125;
	short _NumEstrellas = 1000;

	//Planetas
	vector<State> _estadoParticulas;
	vector <Particle3d> _particulas;

	//Estrellas
	vector<Estrella> _estrellas;
	vector<State> _estadoEstrellas;

	//Recorrido
	vector<vector<Vector3d>> _posParticulas;

	//Metodos constructores de escenas
	void createPlanetExample();
	void createSpiralExample();
	void createCollisionExample();

	//Metodo auxiliar de los constructores de escena
	void initParticlesID();
	void resizeVector();
};

