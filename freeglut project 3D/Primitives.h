#ifndef _H_Primitives_H_
#define _H_Primitives_H_

#include "Includes.h"
/*
--------------------------------------------------------------------------------------
	ESTE .H CONTIENE TODAS LAS ESTRUCTURAS Y CLASES UTILIZABLES POR EL SIMULADOR
--------------------------------------------------------------------------------------
*/

//DECLARACIÓN DE CLASES Y ESTRUCTURAS
class Vector3d;
class Particle3d;
class Objecto3D;
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


/*
--------------------------------------------------------------------------------------
       CLASE OBJECTO3D - CONTIENE TODAS LAS OPERACIONES APLICABLES A UN VECTOR
--------------------------------------------------------------------------------------
*/

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
	TAfin* mT;//Contiene la matriz por la que es necesario post-multiplicar la de modelado-vista antes de dibujarlo
	COLOR color;
	bool visible;
	//...

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
	double x, y, z;//Coordenadas del vector

	//Getters
	GLfloat getX(){ return x; }
	GLfloat getY(){ return y; }
	GLfloat getZ(){ return z; }
	

};

/*
--------------------------------------------------------------------------------------
					CLASE Y MÉTODO AUXILIAR DE PARTÍCULA 3D
--------------------------------------------------------------------------------------
*/
class Particle3d : public Objeto3D
{
public:
	int id;

	Vector3d position, velocity, acceleration;
	//COSAS FISICAS
	double damping; 
	double inverseMass;
	double mass;
	double radius;
	
	Particle3d(){ radius = 0.5; position.x = (rand() % 500 - 250.0) / 10.0; position.y = (rand() % 500 - 250.0) / 10.0; position.z = (rand() % 500 - 250.0) / 10.0; }
	Particle3d(Vector3d p, GLfloat maxVida, GLfloat minVida);
	void dibuja();
	void update(GLfloat dt);			//Update de una partícula

private:
	int seconds_;								//Cuenta la vida de la particula
	const int DeadTriggerUp = 300;
	const int DeadTriggerDown = 0;
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
#endif