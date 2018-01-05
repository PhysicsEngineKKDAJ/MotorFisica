#ifndef _Objeto3D_H_
#define _Objeto3D_H_
#include "TAfin.h"
/*
--------------------------------------------------------------------------------------
CLASE OBJECTO3D - CONTIENE TODAS LAS OPERACIONES APLICABLES A UN VECTOR
--------------------------------------------------------------------------------------
*/

struct Color{
	GLdouble r;
	GLdouble g;
	GLdouble b;
	GLdouble a;
};

enum COLOR{
	BLACK, RED, GREEN, BLUE, WHITE, YELLOW, ORANGE, MAGENTA,
	G75, //75% blanco 
	G50, //50% blanco
	G25, //25% blanco
};


//VIRTUAL PURA
class Objeto3D
{
public:
	Objeto3D() : visible(true) {
		mT = new TAfin();
	};
	virtual ~Objeto3D(){
		delete mT;
	};
	//...

	void traslada(Vector3d* v){ mT->traslada(v); };
	void rota(GLfloat a, GLfloat x, GLfloat y, GLfloat z){ mT->rota(a, x, y, z); };
	void escala(Vector3d* v){ mT->escala(v); };

	bool esVisible() const { return visible; };
	void setInvisible() { visible = false; };
	TAfin* getmT() const { return mT; };
	void posiciona(GLfloat doce, GLfloat trece, GLfloat catorce){ mT->posiciona(doce, trece, catorce); };
	virtual void dibuja() = 0;

protected:
	TAfin* mT;//Contiene la matriz por la que es necesario post-multiplicar  la de modelado-vista antes de dibujarlo
	Color color;
	bool visible;
	//...

};
#endif