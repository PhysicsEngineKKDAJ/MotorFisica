#pragma once
#include "Includes.h"


//Este archivo contiene todos los tipos de objetos que podemos crear

enum COLOR{BLACK, RED, GREEN, BLUE, WHITE, YELLOW, ORANGE, MAGENTA,
			G75, //75% blanco 
			G50, //50% blanco
			G25, //25% blanco
		  };

//Configuraciones iniciales posibles
enum  Preset
{
	Preset_id, //Asigna ID
	Preset_radius, //Asigna un radio en función de la masa
	Preset_random_m, //Establece masas aleatorias
	Preset_random_v, //Establece velocidades aleatorias
}; 

//Declaración anticipada de las clases
class Vector3d;
struct State;
class Particle3d;
struct Derivative;
/*
class Object3d;
class Box3d;
struct Face3d;
*/


//Clase Vector, tiene todas las operaciones para calcular la física
class Vector3d{
public:
	Vector3d() :x(0.0), y(0.0), z(0.0){}
	Vector3d(double X, double Y, double Z) : x(X), y(Y), z(Z){}

	double Magnitude(){ return sqrt(x*x + y*y + z*z); }
	void Normalize(){ double magnitude = Magnitude(); (*this) *= (double)1.0 / magnitude; }

	//Producto escalar
	Vector3d operator*(double k){ return Vector3d(x*k, y*k, z*k); }
	friend Vector3d operator*(double k, Vector3d &v){ return v*k; }
	void operator*=(double k){ *this = (*this)*k; }

	//Suma
	Vector3d operator+(Vector3d &v2){ return Vector3d(x + v2.x, y + v2.y, z + v2.z); }
	void operator+=(Vector3d &v2){ *this = (*this) + v2; }

	//Resta
	Vector3d operator-(Vector3d &v2){ return Vector3d(x - v2.x, y - v2.y, z - v2.z); }
	void operator-=(Vector3d &v2){ *this = (*this) - v2; }

	//Setters
	void  set(double X, double Y, double Z){ x = X; y = Y, z = Z; }
	double x, y, z;//Coordenadas del vector
};

//TENEMOS QUE DEFINIR SI QUEREMOS OBJETOS COMO CUBOS
//Objetos no particulas
/*
class Object3d{
public:
	Object3d() :nVertex(0), nFace(0), nVertexNormal(0), vertex(nullptr), face(nullptr), vertexNormal(nullptr){}
	~Object3d(){}

	int nVertex, nFace, nVertexNormal;

	//Vectores dinámicos con cada uno de los vertices, normales y caras
	Vector3d *vertex;
	Vector3d *vertexNormal;
	Face3d *face;

};

class Box : public Object3d
{

};

//Guarda toda la información de las caras
struct Face3d{
int nFaceV;
int *iVertex;
int *iVertexNormal;

Face3d() :nFaceV(0), iVertex(nullptr), iVertexNormal(nullptr){}
};
*/

class Particle3d{
public:
	int id;

	Vector3d position, velocity, acceleration;
	//COSAS FISICAS
	double damping; 
	double inverseMass;
	double mass;
	double radius;
	Particle3d(){ radius = 0.5; position.x = (rand() % 500 - 250.0) / 10.0; position.y = (rand() % 500 - 250.0) / 10.0; position.z = (rand() % 500 - 250.0) / 10.0; }

	void dibuja();
};

//Estrellas
class Estrella : public Particle3d{
public:
	Estrella(){ mass = 10; radius = 0.5; position.x = (rand() % 5000 - 2500.0) / 10.0; position.y = (rand() % 5000 - 2500.0) / 10.0; position.z = (rand() % 5000 - 2500.0) / 10.0; }
};


//Estado del cuerpo
struct State
{
	Vector3d Position, Velocity, Acceleration;
	float mass;
	int id;

};

//NO SE QUE COÑO ESO
struct Derivative
{
	Vector3d dPosition, dVelocity;
};
