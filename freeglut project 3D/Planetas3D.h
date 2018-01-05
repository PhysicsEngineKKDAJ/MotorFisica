#ifndef _H_Planetas3D_H_
#define _H_Planetas3D_H_
#include "Includes.h"
#include "Objeto3D.h"

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
CLASE Y MÉTODO AUXILIAR DE PARTÍCULA 3D
--------------------------------------------------------------------------------------
*/
class Planetas3D : public Objeto3D
{
public:
	int id;

	Vector3d position, velocity, acceleration;
	//COSAS FISICAS
	double mass;
	double radius;

	Planetas3D(){ radius = 0.5; position.x = (rand() % 500 - 250.0) / 10.0; position.y = (rand() % 500 - 250.0) / 10.0; position.z = (rand() % 500 - 250.0) / 10.0; }
	void dibuja();
};

/*
--------------------------------------------------------------------------------------
CLASE ESTRELLA - HEREDA DE LA CLASE PARTÍCULA 3D
--------------------------------------------------------------------------------------
*/
class Estrella : public Planetas3D{
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

#endif