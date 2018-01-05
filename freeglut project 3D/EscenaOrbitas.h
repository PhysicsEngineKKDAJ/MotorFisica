#pragma once
#include "Escena.h"


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

	short _NumPlanetas;
	short _NumEstrellas;

	//Planetas
	vector<State> _estadoPlanetas;
	vector <Planetas3D> _planetas;

	//Estrellas
	vector<Estrella> _estrellas;
	vector<State> _estadoEstrellas;

	//Recorrido
	vector<vector<Vector3d>> _posPlanetas;

	//Metodos constructores de escenas
	void createPlanetExample();
	void createSpiralExample();
	void createCollisionExample();

	//Metodo auxiliar de los constructores de escena
	void initParticlesID();
	void resizeVector();
};

