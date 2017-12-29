#pragma once
#include "Includes.h"

/*
--------------------------------------------------------------------------------------
	ESTE .H CONTIENE TODAS LAS ESTRUCTURAS Y CLASES UTILIZABLES POR EL SIMULADOR
--------------------------------------------------------------------------------------
*/

//DECLARACIÓN DE CLASES Y ESTRUCTURAS
class Vector3d;
class Particle3d;
struct State;
struct Derivative;
/*
class Object3d;
class Box3d;
struct Face3d;
*/

/*
--------------------------------------------------------------------------------------
									ENUMERADOS
--------------------------------------------------------------------------------------
*/
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

/*
--------------------------------------------------------------------------------------
					CLASE Y MÉTODO AUXILIAR DE PARTÍCULA 3D
--------------------------------------------------------------------------------------
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


/*
--------------------------------------------------------------------------------------
				CLASE ESTRELLA - HEREDA DE LA CLASE PARTÍCULA 3D
--------------------------------------------------------------------------------------
*/
class Estrella : public Particle3d{
public:
	Estrella(){ mass = 10; radius = 0.5; position.x = (rand() % 5000 - 2500.0) / 10.0; position.y = (rand() % 5000 - 2500.0) / 10.0; position.z = (rand() % 5000 - 2500.0) / 10.0; }
};


/*
--------------------------------------------------------------------------------------
									ESTRUCTURAS
--------------------------------------------------------------------------------------
*/

//Struct del estado de un cuerpo - Contiene Posición, Velocidad, Aceleración, Masa y un Identificador Numérico
struct State
{
	Vector3d Position, Velocity, Acceleration;
	float mass;
	int id;

};

//Struct de la derivada de Posición y Velocidad
struct Derivative
{
	Vector3d dPosition, dVelocity;
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
