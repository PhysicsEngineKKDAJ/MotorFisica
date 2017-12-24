#include "stdafx.h"
#include "Physics_Functions.h"
#include <stdio.h>
#include <math.h>
#include <string>
#include <time.h>
#include <GL/freeglut.h>

//--------------CAMARA-------------

//N Y F son 0 por el gluPerspective
GLdouble xRight = 0, xLeft = 800, yTop = 600, yBot = 0, N = 0, F = 0;
int WIDTH = 800, HEIGHT = 600;

//Movimiento de la cámara
int downX, downY;//Movimiento
float sphi = 30.0, stheta = -30.0, sheight = 0, shor = 0;
float sdepth = 20;

//--------------CAMARA-------------

//-------------LUZ---------------

GLfloat lightPos[] = { 1, 1, 1, 0.0 };

//-------------LUZ---------------

//---------------CONTROLES---------------------

//RATON
bool leftButton = false, middleButton = false, rightButton = false; 

//Teclas
bool leftArrow = false, rightArrow = false;

//---------------CONTROLES---------------------

bool paused = true;
//HAY QUE USARLO
//int contEscena;
//Escena*escena;

//ESTA MIERDA HAY QUE QUITARLA DE AQUÍ
vector<vector<Vector3d>> positions(PARTICLE_C);
vector<vector<Vector3d>> p_positions(POINT_PARTICLE_C);

//Dibuja los ejes de coordenadas
void dibujaEjes(){
	glLineWidth(1.5f);
	// Drawing axes
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(5, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 5, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 5);
	glEnd();

}

//CREA TODOS LOS OBJETOS DE LA ESCENA
void buildSceneObjects()
{
	//escena = new Escena(4000);
	//contEscena = 0;

	//TODO LO QUE HAY A PARTIR DE AQUÍ DEBERÍA ESTAR EN LA CONSTRUCTORA DE ESCENA

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

}

//INIT DE OPENGL
//Igual que en IG excepto por la cámara
void InitGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//-----------SHADERS-------------
	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.9f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	//Define el modelo de sombreado: GL_FLAT / GL_SMOOTH (suave)
	glShadeModel(GL_SMOOTH);// Shading by default
	//-----------SHADERS-------------

	buildSceneObjects();

	//--------------LUZ-------------
	//Activa el modelo de iluminación /disable
	glEnable(GL_LIGHTING);

	//Enciende una luz particular /disable
	glEnable(GL_LIGHT0);

	//Definir componentes difusa, especular y ambiente
	GLfloat d[] = { 0.7f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, d);//Luz que vuelve a irradiar la superficie en todas las direcciones.
	GLfloat a[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, a);//Luz que alcanza una superficie aunque no esté expuesta a la fuente de luz.
	GLfloat s[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, s);//Luz que refleja la superficie.
	//GLfloat p[] = { 25.0f, 25.0f, 25.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, p);//Define la posicion de una fuente de luz. ultimo parámetro: 1 local, 0 direccional
	
	//luz ambiente
	/*
	GLfloat amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	*/
	//--------------LUZ-------------

	//-------------CAMARA-----------
	/*
		Hacer el setup igual que en IG lo rompe por el movimiento con el ratón,
		lo dejamos tal como está
	*/
	//-------------CAMARA-----------

}

real deltat = 1000;					//Se usa en lo de integrar
float fac = 0;
int ifac = 0;

//Se usan para debug
float r = 0;                     //NO SE USA
float tp = 0.0;                           //Se va sumando en el render

//Dibuja
void display()
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

	if (rightArrow)
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
	if (leftArrow)
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

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

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

	glutPostRedisplay();
}


//Igual que en IG excepto por el gluPerspective, permite alejar la camara con el raton
void resize(int newWidth, int newHeight) {
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

	//Funcion nueva para que funcione lo de alejar la camara
	gluPerspective(60, RatioViewPort, 1.0, 100000000000.0);
}

//---------------------------CONTROLES--------------------------------

//Se le llama con la pulsación del ratón
void mouse(int button, int state, int x, int y)
{
	downX = x; downY = y;
	leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));

	middleButton = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));

	rightButton = ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN));
}

//Se le llama si se mantiene una tecla (botones del raton)
//Movimiento de la camara 
void motion(int x, int y)
{
	//Rota la cámara respecto al centro
	if (leftButton)
	{
		sphi += (float)(x - downX) / 4.0;
		stheta += (float)(downY - y) / 4.0;
	}

	//Aleja o acerca la cámara
	if (rightButton)
	{
		if (sdepth <= 2 && sdepth >= -2)
			sdepth += (float)(downY - y);

		else
			sdepth += (float)(downY - y)*(abs(sdepth)) / 50.0;
	}

	//Permite mover la cámara en el plano
	if (middleButton)
	{
		sheight += (float)(downY - y)*(abs(sdepth)) / 120.0;
		shor += (float)(downX - x)*(abs(sdepth)) / 120.0;
	}

	downX = x;   downY = y;
}

//Teclas normales
void key(unsigned char key, int x, int y){
	bool need_redisplay = true;
	switch (key) {
	case 27:  /* Escape key */
		//continue_in_main_loop = false; // (**)
		//Freeglut's sentence for stopping glut's main loop (*)
		glutLeaveMainLoop();
		break;

		//case '3': contEscena++; contEscena %= 2; break;//Rota la cámara alrededor del ejeX

	//Permiten mover la altura de la luz
	case 'w': 
		lightPos[1] += 0.1;
		break;
	case 's':
		lightPos[1] -= 0.1;
		break;

	default:
		need_redisplay = false;
		break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}

//Flechas direccionales
void SpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		paused = !paused;
		break;

		//Baja la posición de la luz
	case GLUT_KEY_DOWN:

		break;

	case GLUT_KEY_RIGHT:
		rightArrow = !rightArrow;
		break;

	case GLUT_KEY_LEFT:
		leftArrow = !leftArrow;
		break;
	}
}

//---------------------------CONTROLES--------------------------------

// ----------------------------------- MAIN -------------------------------------
int main(int argc, char* argv[])
{
	cout << "Starting console..." << endl;
	srand(time(0));

	int my_window; // my window's identifier

	// Initialization
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(420, 60);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInit(&argc, argv);

	// Window construction
	my_window = glutCreateWindow("Physics Engine");

	//CALLBACK REGISTRATION
	glutMotionFunc(motion);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutSpecialFunc(SpecialKey);
	glutKeyboardFunc(key);

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
// ----------------------------------- MAIN -------------------------------------

