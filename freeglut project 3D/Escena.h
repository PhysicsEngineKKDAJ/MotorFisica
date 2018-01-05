#pragma once

//Includes comunes de las escenas
#include "Includes.h"
#include "Primitives.h"
//

const int escena_Max = 2; //ESTO SE CAMBIA SEG�N METAS LAS ESCENAS

class Escena{
public:
	Escena() {
		_sphi = 30;
		_stheta = -30.0;
		_sheight = 0;
		_shor = 0;
		_sdepth = 20;
	};
	~Escena() {};

	virtual void dibuja(){

		//Pone la rotaci�n de la escena correspondiente
		glTranslatef(-_shor, _sheight, -_sdepth);
		glRotatef(-_stheta, 1, 0, 0);
		glRotatef(_sphi, 0, 1, 0);

	}

	//-----------------ROTACION DE ESCENA----------------

	//SE SUPONE QUE ESTO DEBE MODIFCAR PARA TODAS LAS ESCENAS
	void setDown(int downX,int downY){
		_downX = downX;   _downY = downY;
	}

	//Rota la c�mara respecto al centro
	void leftButtonMouse(int x, int y){
		_sphi += (float)(x - _downX) / 4.0;
		_stheta += (float)(_downY - y) / 4.0;
	}

	//Aleja o acerca la c�mara
	void rightButtonMouse(int x, int y){
		if (_sdepth <= 2 && _sdepth >= -2)
			_sdepth += (float)(_downY - y);

		else
			_sdepth += (float)(_downY - y)*(abs(_sdepth)) / 50.0;
	}

	//Permite mover la c�mara en el plano
	void middleButtonMouse(int x, int y){
		_sheight += (float)(_downY - y)*(abs(_sdepth)) / 120.0;
		_shor += (float)(_downX - x)*(abs(_sdepth)) / 120.0;
	}

	//-----------------ROTACION DE ESCENA----------------

	void leftArrowActivate(bool b) { flechaIz = b; }
	void rightArrowActivate(bool b) { flechaDe = b; }

protected:
	//Par�metros iniciales de esta escena
	float _sphi, _stheta, _sheight, _shor;
	float _sdepth;
	int _downX, _downY;

	bool flechaIz = false ,flechaDe = false;
};