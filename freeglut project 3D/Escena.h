#pragma once

//Includes comunes de las escenas
#include "Includes.h"
#include "Planetas3D.h"
//

enum TEscenas{ EEspiral, EColision, EPlanetas, EParticles,ETAM };

class Escena{
public:
	Escena() {
		_sphi = 30;
		_stheta = -30.0;
		_sheight = 0;
		_shor = 0;
		_sdepth = 20;
	};
	virtual ~Escena() {};

	virtual void dibuja(){

		//Pone la rotación de la escena correspondiente
		glTranslatef(-_shor, _sheight, -_sdepth);
		glRotatef(-_stheta, 1, 0, 0);
		glRotatef(_sphi, 0, 1, 0);

	}

	//-----------------ROTACION DE ESCENA----------------

	//SE SUPONE QUE ESTO DEBE MODIFCAR PARA TODAS LAS ESCENAS
	void setDown(int downX,int downY){
		_downX = downX;   _downY = downY;
	}

	//Rota la cámara respecto al centro
	void leftButtonMouse(int x, int y){
		_sphi += (float)(x - _downX) / 4.0;
		_stheta += (float)(_downY - y) / 4.0;
	}

	//Aleja o acerca la cámara
	void rightButtonMouse(int x, int y){
		if (_sdepth <= 2 && _sdepth >= -2)
			_sdepth += (float)(_downY - y);

		else
			_sdepth += (float)(_downY - y)*(abs(_sdepth)) / 50.0;
	}

	//Permite mover la cámara en el plano
	void middleButtonMouse(int x, int y){
		_sheight += (float)(_downY - y)*(abs(_sdepth)) / 120.0;
		_shor += (float)(_downX - x)*(abs(_sdepth)) / 120.0;
	}

	//-----------------ROTACION DE ESCENA----------------

	inline void leftArrowActivate(bool b) { flechaIz = b; }
	inline void rightArrowActivate(bool b) { flechaDe = b; }

	inline bool getXActivate() const { return xActivate; }
	inline bool getZActivate() const { return zActivate; }

	inline void zSetActivate(bool a) { zActivate = a; }
	inline void xSetActivate(bool a) { xActivate = a; }

protected:
	//Parámetros iniciales de esta escena
	float _sphi, _stheta, _sheight, _shor;
	float _sdepth;
	int _downX, _downY;

	bool flechaIz = false ,flechaDe = false;
	//Controles extra 
	bool zActivate = false, xActivate = false;
};