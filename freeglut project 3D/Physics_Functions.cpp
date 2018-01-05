#include "Physics_Functions.h"

//No se eliminar estas variables globales
Planetas3D *P;	//array of particles to reference(p3d or pp3d)
short P_C;


double b_g = 6.67384*0.000080; // el 6.67384 es la constante de gravitación universal

Vector3d accUniversalGravitation(State ip)
{
	Vector3d ACCELERATION;
	Vector3d tempAcceleration;
	
	for(int i=0; i<P_C; i++)
	{
		// Salta a la siguiente iteracion
		// Supongo que es para ahorrarse comprobar con la propuesta y las anteriores.
		//SUDA, SE CAMBIA ESTO Y FUERA
		if(ip.id==i) continue;
		
		// supongo que distancia
		double d=sqrt( (ip.Position.x-P[i].position.x)*(ip.Position.x-P[i].position.x) +
				 (ip.Position.y-P[i].position.y)*(ip.Position.y-P[i].position.y) +
				 (ip.Position.z-P[i].position.z)*(ip.Position.z-P[i].position.z)
			   );

		double wk=1.0;
		double f=-b_g*wk*ip.mass*P[i].mass/(d*d);

	
		tempAcceleration.set((ip.Position.x - P[i].position.x)*f / d / ip.mass, (ip.Position.y - P[i].position.y)*f / d / ip.mass,
			(ip.Position.z-P[i].position.z)*f/d/ip.mass);
		ACCELERATION+=tempAcceleration;
	
	}
	
	return ACCELERATION;

}

Vector3d accDirectionalGravity(State sip)	{	return Vector3d(0,-10,0);	}


// evaluar es que le pasas un indice y le aplica las ecuaciones a la posicion y velocidad.
Derivative evaluate(int ip, Vector3d(*acc_func)(State) )
{
	Derivative output;
	output.dPosition = P[ip].velocity ;
	output.dVelocity = acc_func(PlanetToState(P[ip])); //arbitrary acceleration function

	return output;
}

// Sobrecarga de la evaluacion, una forma mas compleja
Derivative evaluate(int ip, float dt, Derivative &d, Vector3d(*acc_func)(State))
{
	State state1;
	Derivative output;
	state1.id=ip;
	state1.mass=P[ip].mass;
	// dp (derivada de la aceleracion), dt (derivada del tiempo)
	state1.Position = P[ip].position + d.dPosition*dt + d.dVelocity*0.5*dt*dt;
	state1.Velocity = P[ip].velocity + d.dVelocity*dt;//p''=dv/dt= acceleration
	
	output.dPosition = state1.Velocity;
	output.dVelocity= acc_func(state1); //arbitrary acceleration function

	return output;
}

//SE USA CUANDO PULSAS CLICK IZQUIERDO
void DirectionalGravity(int ip, Planetas3D* p, short p_c, State& fstate, float dt)
{
	P = p;
	P_C = p_c;
	Vector3d(*acc_func)(State) = accDirectionalGravity;

	// Menudos triples me voy a marcar ahora, atentos
	Derivative k1 = evaluate(ip, acc_func); // determina la curva de la funcion
	Derivative k2 = evaluate(ip, dt * 0.5f, k1, acc_func); // creo que son los puntos de inflexion
	Derivative k3 = evaluate(ip, dt * 0.5f, k2, acc_func); // es una constante
	Derivative k4 = evaluate(ip, dt, k3, acc_func); // tiene que dar 0 para que sea correcto

	Vector3d dPOSITION = 1.0f / 6.0f * (k1.dPosition + 2.0f * (k2.dPosition + k3.dPosition) + k4.dPosition);
	Vector3d dVELOCITY = 1.0f / 6.0f * (k1.dVelocity + 2.0f * (k2.dVelocity + k3.dVelocity) + k4.dVelocity);

	fstate.Position += dPOSITION * dt;
	fstate.Velocity += dVELOCITY * dt;
	fstate.Acceleration = dVELOCITY * dt;

}

// Solo tiene una diferencia con el metodo anterior
void UniversalGravitation(int ip, Planetas3D* p, short p_c, State& fstate, float dt, double G)
{
	P = p;
	P_C = p_c;

	// Esta es la unica diferencia
	if (G >= 0)
		b_g = G;

	// igual que en el metodo de arriba
	Vector3d(*acc_func)(State) = accUniversalGravitation;
	Derivative k1 = evaluate(ip, acc_func);
	Derivative k2 = evaluate(ip, dt * 0.5f, k1, acc_func);
	Derivative k3 = evaluate(ip, dt * 0.5f, k2, acc_func);
	Derivative k4 = evaluate(ip, dt, k3, acc_func);

	Vector3d dPOSITION = 1.0f / 6.0f*(k1.dPosition + 2.0f*(k2.dPosition + k3.dPosition) + k4.dPosition);
	Vector3d dVELOCITY = 1.0f / 6.0f*(k1.dVelocity + 2.0f*(k2.dVelocity + k3.dVelocity) + k4.dVelocity);

	fstate.Position += dPOSITION*dt;
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;
}



// Se utiliza cuando va a colisionar un objeto con otro
void dImpenetrationResolution(int i, int j, Planetas3D *p, State *ps, Vector3d ContactNormal)
{
	// Comprueba si las particulas estan separadas, solapadas o pegandose.
	// Es como una zona muerta
	double err_l = abs((p[i].radius + p[j].radius) - Distance3d(p[i].position, p[j].position));

	// dos variables que son las magnitudes de las coordenadas de la particula
	double vm1 = p[i].velocity.Magnitude(), vm2 = p[j].velocity.Magnitude();
	
	// ???????????????????
	double tv = 1.0 / (p[j].mass + vm2) + 1.0 / (p[i].mass + vm1);
		
	Vector3d mv = ContactNormal;
	mv.Normalize();

	// a la normal de la colision le multilpicamos el error entre
	mv *= (-err_l / tv);

	ps[i].Position += mv * (1 / (-p[i].mass+vm1)),
	ps[j].Position += mv * (1 / (p[j].mass+vm2));				
}

void CollisionResolution(int p_c, Planetas3D *p, State *ps)
{
	double C = 0.60;
	Vector3d Normal;

	for (int i = 0; i < p_c; i++) {
		for (int j = i; j < p_c; j++) {
			// ponemos el resto del codigo en la condicion y quitamos el continue
			if(i == j)
				continue;

			// distancia entre los objetos
			double d = (Distance3d(ps[i].Position, ps[j].Position) - (p[i].radius + p[j].radius));
			if(d < 0) { 
				Normal.set(ps[i].Position.x - ps[j].Position.x, ps[i].Position.y - ps[j].Position.y,
					ps[i].Position.z - ps[j].Position.z);
				Normal.Normalize();

				// Comprobacion de la colision
				dImpenetrationResolution(i, j, p, ps,Normal);
										
				double separatingVelocity = dotProduct(ps[i].Velocity-ps[j].Velocity, Normal);
				double newSepVelocity = -separatingVelocity * C;
				double deltaVelocity = newSepVelocity - separatingVelocity;
				
				// no lo entiendo muy bien la verdad
				const double totalInverseMass = 1.0/(ps[i].mass) + 1.0/(ps[j].mass);
										
				// Calculate the impulse to apply.
				double impulse = deltaVelocity / totalInverseMass;
				// Find the amount of impulse per unit of inverse mass.
				Vector3d impulsePerIMass = Normal * impulse;
				// Apply impulses: they are applied in the direction of the contact,
				// and are proportional to the inverse mass.
				ps[i].Velocity = ps[i].Velocity + impulsePerIMass * (1 / ps[i].mass);
				ps[j].Velocity = ps[j].Velocity + impulsePerIMass * (-1 / ps[j].mass);
								

				}
				
			
			}
		}
				
		
}

