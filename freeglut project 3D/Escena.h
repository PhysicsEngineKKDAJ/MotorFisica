#pragma once

//Includes comunes de las escenas
#include "Includes.h"
#include "Primitives.h"
#include "EscenaOrbitas.h"
//

const int escena_Max = 2; //ESTO SE CAMBIA SEGÚN METAS LAS ESCENAS

class Escena{
public:
	Escena() {
		sphi = 30;
		stheta = -30.0;
		sheight = 0;
		shor = 0;
		sdepth = 20;
	};
	~Escena() {};

	virtual void dibuja() = 0;

	//NO SE SI HACE FALTA
	virtual void update() = 0;

	//Parámetros iniciales de esta escena
	float sphi, stheta, sheight, shor;
	float sdepth = 20;

	//Metodos que modifiquen los valores de la camara
	//SE SUPONE QUE ESTO DEBE MODIFCAR PARA TODAS LAS ESCENAS
	void leftButtonCamera() {
		//sphi += (float)(x - downX) / 4.0;
		//stheta += (float)(downY - y) / 4.0;
	}
};