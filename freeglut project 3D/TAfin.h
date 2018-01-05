#ifndef _TAfin_H_
#define _TAfin_H_

#include <Windows.h>
#include <gl/GL.h>

#include "Vector3D.h"

//Contiene la matriz por la que es necesario POSTmultiplicar la de modelado vista antes de dibujarlo
class TAfin
{
public:
	TAfin();
	~TAfin(){};

	void traslada(Vector3d* v);
	void rota(GLfloat a, GLfloat x, GLfloat y, GLfloat z);//En sentido antihorario
	void escala(Vector3d* v);
	void reset();

	void postMultiplica(GLfloat* m1);

	inline GLfloat * getM() const { return m; };
	inline void posiciona(GLfloat doce, GLfloat trece, GLfloat catorce){ m[12] = doce; m[13] = trece; m[14] = catorce; };

private:
	GLfloat *m; //Matriz: Enumerados por columnas

};
#endif