#pragma once
#include "Objeto3D.h"
class Particle3D : public Objeto3D
{
public:
	Particle3D(Vector3d p, GLfloat minLife, GLfloat maxLife);
	~Particle3D();
	virtual	void dibuja();						//Dibuja una partícula
	virtual void update(GLfloat dt);			//Update de una partícula

	void setColor(Color aux){ color_ = aux; }

	void setDestroy(bool b) { _eliminar = b; }
	bool getDestroy() { return _eliminar; }

private:
	GLfloat life_;
	GLfloat lifeAct_;

	int size_;
	Vector3d pos_;
	Vector3d vel_;
	Vector3d acc_;
	Color color_;

	int seconds_;								//Cuenta la vida de la particula
	const int DeadTriggerUp = 300;
	const int DeadTriggerDown = 0;
	bool _eliminar = false;

};

