#include "Physics_Functions.h"



//VARIABLES GLOBALES DE LA CLASE
vector<Particle3d> particulas(NumParticulas);
vector<Estrella> estrellas(NumEstrellas);
vector<State> estadoParticulas(NumParticulas);
vector<State> estadoEstrellas(NumEstrellas);



Vector3d(*acc_func)(State); //acceleration function pointer
Vector3d ContactNormal; // Vector de la normal cuando dos objetos se colisionan
Particle3d *P;	//array of particles to reference(p3d or pp3d)
State *PS;

short P_C;
double b_g = 6.67384*0.000080, s_g; // el 6.67384 es la constante de gravitación universal
const short NumEstrellas = 1000;
const short NumParticulas = 125;

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
Derivative evaluate(int ip)
{
	Derivative output;
	output.dPosition = P[ip].velocity ;
	output.dVelocity = acc_func(ParticleToState(P[ip])); //arbitrary acceleration function

	return output;
}

// Sobrecarga de la evaluacion, una forma mas compleja
Derivative evaluate(int ip, float dt, Derivative &d)
{
	State state1, state2;
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

// No se usa en la demo
void DirectionalGravity_simple(int ip, Particle3d* p , short p_c, State& fstate, float dt)
{
	P=p;
	P_C=p_c;
	acc_func=accDirectionalGravity;
	Derivative k1 = evaluate(ip);
	
	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION= k1.dPosition;
	dVELOCITY =k1.dVelocity;
	
	
	fstate.Position += dPOSITION*dt; 
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;

}

//SE USA CUANDO PULSAS CLICK IZQUIERDO
void DirectionalGravity(int ip, Particle3d* p , short p_c, State& fstate, float dt)
{
	P = p;
	P_C = p_c;
	acc_func = accDirectionalGravity;

	// Menudos triples me voy a marcar ahora, atentos
	Derivative k1 = evaluate(ip); // determina la curva de la funcion
	Derivative k2 = evaluate(ip, dt * 0.5f, k1); // creo que son los puntos de inflexion
	Derivative k3 = evaluate(ip, dt * 0.5f, k2); // es una constante
	Derivative k4 = evaluate(ip, dt, k3); // tiene que dar 0 para que sea correcto

	Vector3d dPOSITION = 1.0f / 6.0f * (k1.dPosition + 2.0f * (k2.dPosition + k3.dPosition) + k4.dPosition);
	Vector3d dVELOCITY = 1.0f / 6.0f * (k1.dVelocity + 2.0f * (k2.dVelocity + k3.dVelocity) + k4.dVelocity);

	fstate.Position += dPOSITION * dt;
	fstate.Velocity += dVELOCITY * dt;
	fstate.Acceleration = dVELOCITY * dt;

}

// Solo tiene una diferencia con el metodo anterior
void UniversalGravitation(int ip, Particle3d* p, short p_c, State& fstate , float dt, double G )
{
	P = p;
	P_C = p_c;

	// Esta es la unica diferencia
	if (G >= 0)
		b_g = G;

	// igual que en el metodo de arriba
	acc_func = accUniversalGravitation;
	Derivative k1 = evaluate(ip);
	Derivative k2 = evaluate(ip, dt * 0.5f, k1);
	Derivative k3 = evaluate(ip, dt * 0.5f, k2);
	Derivative k4 = evaluate(ip, dt, k3);

	Vector3d dPOSITION = 1.0f / 6.0f*(k1.dPosition + 2.0f*(k2.dPosition + k3.dPosition) + k4.dPosition);
	Vector3d dVELOCITY = 1.0f / 6.0f*(k1.dVelocity + 2.0f*(k2.dVelocity + k3.dVelocity) + k4.dVelocity);

	fstate.Position += dPOSITION*dt;
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;
}

// Literalmente lo mismo que DirectionalGravity_simple, excepto por (*)
void UniversalGravitation_simple(int ip, Particle3d* p, short p_c, State& fstate , float dt, double G )
{
	P=p;
	P_C=p_c;
	
	// (*)
	if (G >= 0)
		b_g = G;
	// (*)

	acc_func=accUniversalGravitation;
	Derivative k1 = evaluate(ip);
	
	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION= k1.dPosition;
	dVELOCITY = k1.dVelocity;
	
	fstate.Position += dPOSITION*dt; 
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;

}


// Se utiliza cuando va a colisionar un objeto con otro
void dImpenetrationResolution(int i, int j, Particle3d *p, State *ps)
{
	// previous distances i & j
	double di = Distance3d(p[i].position, ps[i].Position);
	double dj = Distance3d(p[j].position, ps[j].Position);

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

void CollisionResolution(int p_c, Particle3d *p, State *ps)
{
	double d1, d2; // estas seran las deltas de las funciones
	double ra, rb; // supongo que son los radios de las esferas
	double err_l;
	double C = 0.60;
	Vector3d Normal, Tangent, Tangent2, Tangent3;

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
				ContactNormal = Normal;

				// Comprobacion de la colision
				dImpenetrationResolution(i, j, p, ps);
										
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

/*
******************************************
******************************************
MÉTODOS EN DESUSO
******************************************
******************************************
*/


/* MÉTODO ENCARGADO DE CALCULAR LAS COLISIONES ENTRE UNA PARTÍCULA Y EL PLANO (SUELO) DE LA SIMULACIÓN */

//IMPORTANTE: NO SE USA ACTUALMENTE EN LA SIMULACIÓN 
void CollisionResolution_Ground(int p_c, Particle3d *p, State *ps){

	//VARIABLES LOCALES
	Vector3d Tangent, Tangent2, Tangent3;
	double v1n, v1t;
	double v1n2, v1t2;
	double v1n3, v1t3;
	double v1np;
	double C=0.45;
	double groundMass=9999999999;
	
	//Normalizamos Vector de la normal global
	ContactNormal.set(0, 1, 0);
	ContactNormal.Normalize();

	for(int i=0; i<p_c; i++)
		{		
			//Si la partícula ha llegado a una posición en Y determinada (< -25)
			if(ps[i].Position.y<-25)	
				{
					//Recalculamos su posición para dejarla en la posición límite
					ps[i].Position.y=-25;

					Vector3d &s1=ps[i].Velocity;
					
					//Damos valor a las 3 tangentes para que estén acordes al plano
					Tangent.set(-ContactNormal.y, ContactNormal.x, 0);
					Tangent2.set(-ContactNormal.z, 0, ContactNormal.x);
					Tangent3.set(0, -ContactNormal.z, ContactNormal.y);
					
					//Suma de las normales del plano
					v1n=ContactNormal.x*s1.x + ContactNormal.y*s1.y + ContactNormal.z*s1.z;

					//Damos valor a la suma de los vectores entre tangentes y estado de partícula
					v1t=Tangent.x*s1.x + Tangent.y*s1.y;
					v1t2=Tangent2.x*s1.x + Tangent2.z*s1.z;
					v1t3=Tangent3.z*s1.z + Tangent3.y*s1.y;

					//Coeficiente de choque
					v1np=(v1n*(ps[i].mass-C*groundMass)+ 0*groundMass*(C+1))/(ps[i].mass+groundMass);
					
					//Añadimos al estado del cuerpo (partícula) la velocidad resultante del choque con el plano (rebote)				
					ps[i].Velocity.x=ContactNormal.x*v1np+ Tangent.x*v1t + Tangent2.x*v1t2 + Tangent3.x*v1t3;
					ps[i].Velocity.y=ContactNormal.y*v1np+ Tangent.y*v1t + Tangent2.y*v1t2 + Tangent3.y*v1t3;
					ps[i].Velocity.z=ContactNormal.z*v1np+ Tangent.z*v1t + Tangent2.z*v1t2 + Tangent3.z*v1t3;

				
			}
		}

}



/*void CollisionResolution2(int p_c, Particle3d *p, State *ps)
{
Vector3d Normal, Tangent, Tangent2, Tangent3;
double ra, rb;
double err_l;
double C=.5;

for(int i=0; i<p_c; i++)
{
for(int j=i; j<p_c; j++)
{
if(i==j)
continue;

double d=(Distance3d(ps[i].Position, ps[j].Position)-(p[i].radius+p[j].radius));
if( d<0)
{
Normal.set(ps[i].Position.x-ps[j].Position.x, ps[i].Position.y-ps[j].Position.y,ps[i].Position.z-ps[j].Position.z);
Normal.Normalize();
ContactNormal=Normal;
dImpenetrationResolution(i, j, p, ps);

Vector3d &s1=ps[i].Velocity, &s2=ps[j].Velocity;

double v1n, v1t, v2n, v2t;
double v1n2, v1t2, v2n2, v2t2;
double v1n3, v1t3, v2n3, v2t3;
double v1np, v2np;


Tangent.set(-Normal.y, Normal.x, 0);
Tangent2.set(-Normal.z, 0, Normal.x);
Tangent3.set(0, -Normal.z, Normal.y);
v1n=Normal.x*s1.x + Normal.y*s1.y + Normal.z*s1.z;
v2n=Normal.x*s2.x + Normal.y*s2.y + Normal.z*s2.z;

v1t=Tangent.x*s1.x + Tangent.y*s1.y;
v2t=Tangent.x*s2.x + Tangent.y*s2.y;
v1t2=Tangent2.x*s1.x + Tangent2.z*s1.z;
v2t2=Tangent2.x*s2.x + Tangent2.z*s2.z;
v1t3=Tangent3.z*s1.z + Tangent3.y*s1.y;
v2t3=Tangent3.z*s2.z + Tangent3.y*s2.y;

v1np=(v1n*(ps[i].mass-C*ps[j].mass)+ v2n*ps[j].mass*(C+1))/(ps[i].mass+ps[j].mass);
v2np=(v2n*(ps[j].mass-C*ps[i].mass)+ v1n*ps[i].mass*(C+1))/(ps[i].mass+ps[j].mass);

ps[i].Velocity.x=Normal.x*v1np+ Tangent.x*v1t + Tangent2.x*v1t2 + Tangent3.x*v1t3;
ps[i].Velocity.y=Normal.y*v1np+ Tangent.y*v1t + Tangent2.y*v1t2 + Tangent3.y*v1t3;
ps[i].Velocity.z=Normal.z*v1np+ Tangent.z*v1t + Tangent2.z*v1t2 + Tangent3.z*v1t3;
ps[j].Velocity.x=Normal.x*v2np+ Tangent.x*v2t + Tangent2.x*v2t2 + Tangent3.x*v2t3;
ps[j].Velocity.y=Normal.y*v2np+ Tangent.y*v2t + Tangent2.y*v2t2 + Tangent3.y*v2t3;
ps[j].Velocity.z=Normal.z*v2np+ Tangent.z*v2t + Tangent2.z*v2t2 + Tangent3.z*v2t3;
}

}

}


}*/