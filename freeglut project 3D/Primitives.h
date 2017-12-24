#pragma once
#include "stdafx.h"

typedef double real; 

/*
Este archivo contiene todos los tipos de objetos que podemos crear

*/

enum COLOR{BLACK, RED, GREEN, BLUE, WHITE, YELLOW, ORANGE, MAGENTA,
			G75, //75% white (GN: shades of grey)
			G50, //50% white
			G25, //25% white
		  };

enum  P3d_preset{p3d_random_all,//random values for all attributes
				p3d_id, //assign ids
				p3d_radius, // set radius according to mass
				p3d_random_m, //set random masses
				p3d_random_v,
				p3d_point_particles, //shapeless particles-- point objects
				p3d_01,
				p3d_02,
				p3d_03,
				p3d_04
				}; //set random velocities

//Declaración anticipada de las clases
struct Face3d;
struct State;
class Vector3d;
class Object3d;
class Box3d;
class Particle3d;

struct Derivative;


//Clase Vector, tiene todas las operaciones para calcular la física
class Vector3d{
public:
	Vector3d() :x(0.0), y(0.0), z(0.0){}
	Vector3d(real X, real Y, real Z) : x(X), y(Y), z(Z){}

	real Magnitude(){ return sqrt(x*x + y*y + z*z); }
	void Normalize(){ real magnitude = Magnitude(); (*this) *= (real)1.0 / magnitude; }

	//Producto escalar
	Vector3d operator*(real k){ return Vector3d(x*k, y*k, z*k); }
	friend Vector3d operator*(real k, Vector3d &v){ return v*k; }
	void operator*=(real k){ *this = (*this)*k; }

	//Suma
	Vector3d operator+(Vector3d &v2){ return Vector3d(x + v2.x, y + v2.y, z + v2.z); }
	void operator+=(Vector3d &v2){ *this = (*this) + v2; }

	//Resta
	Vector3d operator-(Vector3d &v2){ return Vector3d(x - v2.x, y - v2.y, z - v2.z); }
	void operator-=(Vector3d &v2){ *this = (*this) - v2; }

	//Setters
	void  Set(real X, real Y, real Z){ x = X; y = Y, z = Z; }
	real x, y, z;//Coordenadas del vector
};

//Clase padre de todos los objetos
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

//ES UN OBJETO 3D TAL CUAL CREO
class Box : public Object3d
{

};

//Partículas
class Particle3d{
public:
	int id;

	Vector3d position, velocity, acceleration;
	//COSAS FISICAS
	real damping; 
	real inverseMass;
	real mass;
	real radius;
	Particle3d(){ radius = 0.5; position.x = (rand() % 500 - 250.0) / 10.0; position.y = (rand() % 500 - 250.0) / 10.0; position.z = (rand() % 500 - 250.0) / 10.0; }
};

//Estrellas
class PointParticle3d : public Particle3d{
public:
	PointParticle3d(){ mass = 10; radius = 0.5; position.x = (rand() % 5000 - 2500.0) / 10.0; position.y = (rand() % 5000 - 2500.0) / 10.0; position.z = (rand() % 5000 - 2500.0) / 10.0; }
};

//Guarda toda la información de las caras
struct Face3d{
	int nFaceV;
	int *iVertex;
	int *iVertexNormal;

	Face3d() :nFaceV(0), iVertex(nullptr), iVertexNormal(nullptr){}
};

//Estado del cuerpo
struct State
{
	Vector3d Position, Velocity, Acceleration;
	float mass;
	int i;

};

//NO SE QUE COÑO ESO
struct Derivative
{
	Vector3d dPosition, dVelocity;
};
