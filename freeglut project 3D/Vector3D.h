#ifndef _Vector3D_H_
#define _Vector3D_H_

#include <GL/freeglut.h>
#include <math.h>
//TAL VEZ DEBERÍAMOS USAR PUNTOVECTOR?
/*
--------------------------------------------------------------------------------------
CLASE VECTOR - CONTIENE TODAS LAS OPERACIONES APLICABLES A UN VECTOR
--------------------------------------------------------------------------------------
*/
class Vector3d{
public:
	Vector3d() :x(0.0), y(0.0), z(0.0){}
	Vector3d(double X, double Y, double Z) : x(X), y(Y), z(Z){}

	double Magnitude(){ return sqrt(x*x + y*y + z*z); }
	void Normalize(){ double magnitude = Magnitude(); (*this) *= (double)1.0 / magnitude; }

	//Operaciones suma, resta, multiplicación
	Vector3d operator*(double k){ return Vector3d(x*k, y*k, z*k); }
	friend Vector3d operator*(double k, Vector3d &v){ return v*k; }
	void operator*=(double k){ *this = (*this)*k; }

	//Producto Escalar y Vectorial
	GLfloat productoEscalar(Vector3d* vector);
	Vector3d* productoVectorial(Vector3d* vector);

	//Suma
	Vector3d operator+(Vector3d &v2){ return Vector3d(x + v2.x, y + v2.y, z + v2.z); }
	void operator+=(Vector3d &v2){ *this = (*this) + v2; }

	//Resta
	Vector3d operator-(Vector3d &v2){ return Vector3d(x - v2.x, y - v2.y, z - v2.z); }
	void operator-=(Vector3d &v2){ *this = (*this) - v2; }

	//Clonar Vector
	Vector3d* clonar() { return new Vector3d(x, y, z); }

	//Setters
	void  set(double X, double Y, double Z){ x = X; y = Y, z = Z; }

	//Getters
	GLfloat getX(){ return x; }
	GLfloat getY(){ return y; }
	GLfloat getZ(){ return z; }

	/*METODOS DE PUNTOVECTOR
	GLfloat getA();
        bool esPunto();
        bool esVector();
        void escalar(GLfloat factor);
        void normalizar();;*/
	GLfloat x, y, z;//Coordenadas del vector

private:
	//int pv;

};
#endif