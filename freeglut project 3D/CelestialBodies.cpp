//INCLUDES
#include "stdafx.h"
#include "Physics_Functions.h"
#include <stdio.h>
#include <math.h>
#include <string>
#include <time.h>
#include <GL/freeglut.h>

//OBJETOS

//VALORES NUMÉRICOS
int downX, downY;
float lightx = 1, lighty = 1, lightz = 1;
float sphi = 30.0, stheta = -30.0, sheight = 0, shor = 0;
float sdepth = 20;

//BOOLS
bool leftButton = false, middleButton = false, rightButton = false; //RATON
bool FlechaIz = false;
bool FlechaDer = false;
bool paused = true;

//Métodos
void Render();
void Idle();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void SpecialKey(int k, int x, int y);

//VECTORES DE COSAs
vector<vector<Vector3d>> positions(PARTICLE_C);
vector<vector<Vector3d>> p_positions(POINT_PARTICLE_C);

//Acceleration caused by particles in P on ip
Vector3d acceleration(State ip, vector<Particle3d> P, float t)
{
	Vector3d ACCELERATION;                      //Aceleración
	Vector3d tempAcceleration;                  //Aceleración temportal

	//Por cada particula, halla algo y luego suma una aceleración temporal a una aceleración
	for (int i = 0; i<PARTICLE_C; i++)
	{
		if (ip.i = !i){

			//Cálculos del cuadrado de algo
			real d = sqrt((ip.Position.x - P[i].position.x)*(ip.Position.x - P[i].position.x) +
				(ip.Position.y - P[i].position.y)*(ip.Position.y - P[i].position.y) +
				(ip.Position.z - P[i].position.z)*(ip.Position.z - P[i].position.z)
				);

			real f = -6.67384*0.000000180 * 10 * ip.mass*P[i].mass / (d*d); //Constante bastante interesante

			//ACUMULA LAS ACELERACIONES, DE LA QUE LLEVA MAS LA TEMPORAL
			tempAcceleration.Set((ip.Position.x - P[i].position.x)*f / d / ip.mass, (ip.Position.y - P[i].position.y)*f / d / ip.mass,
				(ip.Position.z - P[i].position.z)*f / d / ip.mass);
			ACCELERATION += tempAcceleration;//accumulate accerleations
		}
	}

	return ACCELERATION;
}

//Hace la derivada de algo ahí to guapo
Derivative evaluate(int ip, vector<Particle3d> P, float t)
{
	Derivative output;
	output.dPosition = P[ip].velocity;

	output.dVelocity = acceleration(P3dToState(P[ip]), P, t);
	return output;
}

//Hace la derivada de algo ahi to guapo basandose en la posicion y velocidad de otra derivada
//Segunda derivada??
Derivative evaluate(int ip, vector<Particle3d> P, float t, float dt, Derivative &d)
{
	State state1;

	Derivative output;
	state1.i = ip;
	state1.mass = P[ip].mass;
	state1.Position = P[ip].position + d.dPosition*dt;// dp/dt= velocity
	state1.Velocity = P[ip].velocity + d.dVelocity*dt;//p''=dv/dt= acceleration

	output.dPosition = state1.Velocity;

	output.dVelocity = acceleration(state1, P, t + dt);

	return output;
}

//Integra y tal
void integrate(int ip, vector<Particle3d> P, float t, float dt)
{
	Derivative k1 = evaluate(ip, P, t);
	Derivative k2 = evaluate(ip, P, t, dt*0.5f, k1);
	Derivative k3 = evaluate(ip, P, t, dt*0.5f, k2);
	Derivative k4 = evaluate(ip, P, t, dt, k3);

	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION = 1.0f / 6.0f*(k1.dPosition + 2.0f*(k2.dPosition + k3.dPosition) + k4.dPosition);
	dVELOCITY = 1.0f / 6.0f*(k1.dVelocity + 2.0f*(k2.dVelocity + k3.dVelocity) + k4.dVelocity);

	p3d_states[ip].Position += dPOSITION*dt;
	p3d_states[ip].Velocity += dVELOCITY*dt;
	p3d_states[ip].Acceleration = dVELOCITY*dt;
}

void dibujaEjes(){
	glLineWidth(2.5);
	glPointSize(5);
	// Drawing axes
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();

}

//CREA TODOS LOS OBJETOS DE LA ESCENA
void buildSceneObjects()
{
	//escena = new Escena(4000);

	InitP3d(particles.data(), PARTICLE_C, p3d_id);
	InitP3d(point_particles.data(), POINT_PARTICLE_C, p3d_id);

	InitP3d(particles.data(), PARTICLE_C, p3d_random_m);

	//EL CHOQUECITO
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{

				particles[i * 25 + j*5 + k].position.Set(i * 3, j * 3,k*3);
				particles[i * 25 + j * 5 + k].velocity.Set(0, 0, 0);
				particles[i * 25 + j * 5 + k].mass = 5000 + i * 25 + j*5 + k;
				particles[i * 25 + j * 5 + k].radius = 1;
			}
		}

	}

	particles[0].position.Set(5, 5, 100);
	particles[0].velocity.Set(0, 0, -1000);
	particles[0].mass = 1000000;
	particles[0].radius = 10;
	/*PLANETAS ORBITANDO
	//test:
	particles[0].mass=1000000000;
	particles[0].radius = 20;
	particles[0].position.Set(0, 0, 0);
	
	particles[1].position.Set(200, 0, 0);
	particles[1].velocity.Set(0, 0, 150);
	particles[1].mass = 10000;
	particles[1].radius = 5;

	particles[2].position.Set(0, 0, 100);
	particles[2].velocity.Set(220, 0, 0);
	particles[2].mass = 100;
	particles[2].radius= 3;

	particles[3].position.Set(0, 100, 100);
	particles[3].velocity.Set(0, -300, -100);
	particles[3].mass = 1000;
	particles[3].radius = 1;

	particles[4].position.Set(100, 100, 100);
	particles[4].velocity.Set(0, -0, -100);
	particles[4].mass = 100000;
	particles[4].radius = 1;
	*/

	/*
	particles[5].position.Set(0, 200, 000);
	particles[5].velocity.Set(0, -100, 0);
	particles[5].mass = 1000;
	particles[5].radius = 4;


	particles[6].position.Set(0, 100, 0);
	particles[6].velocity.Set(0, 200, 0);
	particles[6].mass = 10000;
	particles[6].radius = 8;
	*/

	CopyParticles(p3d_states.data(), particles.data(), PARTICLE_C);
	CopyParticles(p_p3d_states.data(), point_particles.data(), POINT_PARTICLE_C);

	for (int i = 0; i<PARTICLE_C; i++)
		cout << i << " m: " << particles[i].mass << ":\n vx= " << particles[i].velocity.x << ", vy= " << particles[i].velocity.y << ", vz= " << particles[i].velocity.z << endl;

	//contEscena = 0;
}

//INIT DE OPENGL
void InitGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	GLfloat mat_specular[] = { 0.20, 0.250, .250, .250 };
	GLfloat mat_shininess[] = { 40.0 };

	//glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);

	//Define el modelo de sombreado: GL_FLAT / GL_SMOOTH (suave)
	glShadeModel(GL_SMOOTH);

	buildSceneObjects();

	//Activa el modelo de iluminación /disable
	glEnable(GL_LIGHTING);

	//Enciende una luz particular /disable
	glEnable(GL_LIGHT0);

	/*
	//Definir componentes difusa, especular y ambiente
	GLfloat d[] = { 0.7f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, d);//Luz que vuelve a irradiar la superficie en todas las direcciones.
	GLfloat a[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, a);//Luz que alcanza una superficie aunque no esté expuesta a la fuente de luz.
	GLfloat s[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, s);//Luz que refleja la superficie.
	GLfloat p[] = { 25.0f, 25.0f, 25.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, p);//Define la posicion de una fuente de luz. ultimo parámetro: 1 local, 0 direccional
	*/

	/*
	// Camera set up
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

	// Frustum set up
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xLeft, xRight, yBot, yTop, N, F);
	//glOrtho(xLeft, xRight, yBot, yTop, N, F);

	// Viewport set up
	glViewport(0, 0, WIDTH, HEIGHT);
	*/

	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	//glOrtho(-1,1,1,-1,-2,2);
	//glEnable          (GL_BLEND);
	//glBlendFunc           (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glHint            (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// glutFullScreen      ( );  
}

//VARIABLES HUERFANAS
real deltat = 1000;					//Se usa en lo de integrar
//SE USAN
float fac = 0;
int ifac = 0;

//Más variables huerfanas :(
//Se usan para debug
float r = 0;                     //NO SE USA
float tp = 0.0;                           //Se va sumando en el render

//RENDER
void Render()
{
	glEnable(GL_LIGHTING);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor (0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.5, 0, 0);

	glPushMatrix();

	glTranslatef(-shor, sheight, -sdepth);
	glRotatef(-stheta, 1, 0, 0);
	glRotatef(sphi, 0, 1, 0);

	//Otro puto tiempo
	static int t = clock();

	if (FlechaDer)
	{
		//Constante que usas en la gravitacion universal
		real G = 6.67384*0.00080;

		for (int i = 0; i < PARTICLE_C; i++)
		{
			//La G la usas aquí
			UniversalGravitation(i, particles.data(), PARTICLE_C, p3d_states[i], 1.0 / deltat, G);
		}
	}

	//?????
	if (FlechaIz)
	{
		for (int i = 0; i < PARTICLE_C; i++)//Calcula la gravedad direccional de cada una de las particulas
			DirectionalGravity(i, particles.data(), PARTICLE_C, p3d_states[i], 1.0 / deltat);
		
	}
	//Resuelve las colisiones que pudieran haber
	CollisionResolution(PARTICLE_C, particles.data(), p3d_states.data());

	//Copia el estado
	CopyStates(p3d_states.data(), particles.data(), PARTICLE_C);

	//Dibuja las particulas
	for (int i = 0; i<PARTICLE_C; i++)
		DrawParticle3d(particles[i]);
	
	glDisable(GL_LIGHTING);
	GLfloat light_position[] = { lightx, lighty, lightz, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	SetColor(GREEN);

	
	//---------------POINT PARTICLE---------------
	for (int i = 0; i<POINT_PARTICLE_C; i++)
	{
		if (p_positions[i].size()>100)
			p_positions[i].erase(p_positions[i].begin(), p_positions[i].begin() + 1);
	}

	for (int i = 0; i<POINT_PARTICLE_C; i++)
		p_positions[i].push_back(Vector3d(point_particles[i].position.x, point_particles[i].position.y, point_particles[i].position.z));

	for (int i = 0; i<POINT_PARTICLE_C; i++)
	{
		SetColor(BLACK);
		glBegin(GL_POINTS);

		glVertex3f(p_positions[i][0].x, p_positions[i][0].y, p_positions[i][0].z);
		glEnd();

		glPointSize(1);
		glBegin(GL_POINTS);


		for (int j = 1; j<p_positions[i].size(); j++)
		{
			SetColor(G75);
			glColor4f(float(j) / (float(p_positions[i].size())), float(j) / (float(p_positions[i].size())), float(j) / (float(p_positions[i].size())), float(j) / (float(p_positions[i].size())));
			glVertex3f(p_positions[i][j].x, p_positions[i][j].y, p_positions[i][j].z);
		}
		glEnd();

	}
	//---------------POINT PARTICLE---------------

	//---------------PARTICLE---------------

	for (int i = 0; i<PARTICLE_C; i++)
	{
		if (positions[i].size()>2600000)
			positions[i].erase(positions[i].begin(), positions[i].begin() + 1);
	}

	for (int i = 0; i<PARTICLE_C; i++)
		positions[i].push_back(Vector3d(particles[i].position.x, particles[i].position.y, particles[i].position.z));

	for (int i = 0; i<PARTICLE_C; i++)
	{
		glBegin(GL_POINTS);

		glVertex3f(positions[i][0].x, positions[i][0].y, positions[i][0].z);
		glEnd();

		glPointSize(1);
		glBegin(GL_POINTS);

		SetColor(WHITE);
		for (int j = 1; j<positions[i].size(); j++)
			//glVertex3f(positions[i][j].x, positions[i][j].y, positions[i][j].z);
		
		glEnd();
	}

	//---------------PARTICLE---------------

	glDisable(GL_LIGHTING);
	glClear(GL_DEPTH_BUFFER_BIT);

	/*
	switch (contEscena)
	{
	case 0: test1.dibuja(); break;
	case 1: testRB1.dibuja(); break;

	default:
		break;

	}*/

	
	//-------------DEBUG----------------
	
	//Draw velocity and acceleration vectors
	SetColor(GREEN);
	for (int i = 0; i<PARTICLE_C; i++)
		DrawVector3d(particles[i].velocity*pow(1.89, ifac*-1), particles[i].position.x, particles[i].position.y, particles[i].position.z);

	SetColor(RED);
	for (int i = 0; i<PARTICLE_C; i++)
		DrawVector3d(particles[i].acceleration*pow(3.1, ifac*-1), particles[i].position.x, particles[i].position.y, particles[i].position.z);
		
	dibujaEjes();

	//-------------DEBUG----------------
	
	//escena->dibuja();
	glPopMatrix();

	tp += 0.01;

	glFlush();
	glutSwapBuffers();

	//glutPostRedisplay();
}

//PA ESO PONLO EN EL RENDER
void Idle()
{
	glutPostRedisplay();
}

//Movimiento de la camara
void motion(int x, int y)
{
	static int temp_x = x, temp_y = y;
	static float margin = 0.05;

	if (leftButton)
	{
		sphi += (float)(x - downX) / 4.0;
		stheta += (float)(downY - y) / 4.0;
	}
	if (rightButton){
		if (sdepth <= 2 && sdepth >= -2)
			sdepth += (float)(downY - y);

		else sdepth += (float)(downY - y)*(abs(sdepth)) / 50.0;
		//cout<<sdepth<<": "<<downY<<" - "<<y<<endl;

	} // scale


	if (middleButton)
	{
		sheight += (float)(downY - y)*(abs(sdepth)) / 120.0;
		shor += (float)(downX - x)*(abs(sdepth)) / 120.0;
	}

	downX = x;   downY = y;
}

//Resize
void reshape(int newWidth, int newHeight) {
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newWidth);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, 0, 0);
	gluPerspective(60, (GLfloat)newWidth / (GLfloat)newWidth, 1.0, 100000000000.0);
	glMatrixMode(GL_MODELVIEW);

	/* CON ESTA MIERDA ARREGLARIAMOS EL RESIZE PARA 
	WIDTH = newWidth;
	HEIGHT = newHeight;
	GLdouble RatioViewPort = (float)WIDTH / (float)HEIGHT;
	glViewport(0, 0, WIDTH, HEIGHT);

	GLdouble SVAWidth = xRight - xLeft;
	GLdouble SVAHeight = yTop - yBot;
	GLdouble SVARatio = SVAWidth / SVAHeight;
	if (SVARatio >= RatioViewPort) {
		GLdouble newHeight = SVAWidth / RatioViewPort;
		GLdouble yMiddle = (yBot + yTop) / 2.0;
		yTop = yMiddle + newHeight / 2.0;
		yBot = yMiddle - newHeight / 2.0;
	}
	else {
		GLdouble newWidth = SVAHeight*RatioViewPort;
		GLdouble xMiddle = (xLeft + xRight) / 2.0;
		xRight = xMiddle + newWidth / 2.0;
		xLeft = xMiddle - newWidth / 2.0;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xLeft, xRight, yBot, yTop, N, F);
	*/
}

//---------------------------CONTROLES--------------------------------

void mouse(int button, int state, int x, int y)
{
	downX = x; downY = y;
	leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));

	middleButton = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));

	rightButton = ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN));
}

void key(unsigned char key, int x, int y){
	bool need_redisplay = true;
	switch (key) {
	case 27:  /* Escape key */
		//continue_in_main_loop = false; // (**)
		//Freeglut's sentence for stopping glut's main loop (*)
		glutLeaveMainLoop();
		break;

		//case '3': contEscena++; contEscena %= 2; break;//Rota la cámara alrededor del ejeX
	default:
		need_redisplay = false;
		break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}

void SpecialKey(int k, int x, int y)
{
	switch (k)
	{
	case GLUT_KEY_UP:

		//lighty+=0.1;
		if (paused)
			paused = false;
		else
			paused = true;
		break;

		//BAJA LA POS DE LA LUZ
	case GLUT_KEY_DOWN:
		lighty -= 0.1;

		break;

	case GLUT_KEY_RIGHT:
		FlechaDer = !FlechaDer;
		break;

	case GLUT_KEY_LEFT:
		//lightz-=0.1;
		FlechaIz = !FlechaIz;
		break;
	}
}

//---------------------------CONTROLES--------------------------------

// ----------------------------------- MAIN -------------------------------------
int _tmain(int argc, char* argv[])
{
	cout << "Starting console..." << endl;
	srand(time(0));

	int my_window; // my window's identifier

	// Initialization
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(420, 60);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInit(&argc, argv);

	// Window construction
	my_window = glutCreateWindow("Physics Engine");

	//CALLBACK REGISTRATION
	//glutMotionFunc(motion);
	//glutMouseFunc(mouse);
	//glutPassiveMotionFunc(passive);
	glutMotionFunc(motion);
	glutDisplayFunc(Render);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutSpecialFunc(SpecialKey);
	glutKeyboardFunc(key);
	// glutSpecialFunc     ( arrow_keys );
	glutIdleFunc(Idle);

	InitGL();

	glutMainLoop();

	// We would never reach this point using classic glut
	system("PAUSE");

	/*
	delete escena;
	escena = nullptr;
	*/

	return 0;
}
//-----------------------------------------------------------------------
