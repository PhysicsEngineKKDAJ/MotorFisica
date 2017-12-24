#include "Includes.h"
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
//Este vector guarda todas las posiciones antiguas de cada particula
vector<vector<Vector3d>> posParticulas(NumParticulas);

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

	//Da un ID a todas las particulas
	InitParticles(particulas, NumParticulas, Preset_id);

	//Da un ID a todas las estrellas
	InitParticles(estrellas, NumEstrellas, Preset_id);

	//Crea todas las partículas con una masa aleatoria
	InitParticles(particulas, NumParticulas, Preset_random_m);

	
	//EL CHOQUECITO
	/*
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{

				particulas[i * 25 + j*5 + k].position.Set(i * 3, j * 3,k*3);
				particulas[i * 25 + j * 5 + k].velocity.Set(0, 0, 0);
				particulas[i * 25 + j * 5 + k].mass = 5000 + i * 25 + j*5 + k;
				particulas[i * 25 + j * 5 + k].radius = 1;
			}
		}

	}

	particulas[0].position.Set(5, 5, 100);
	particulas[0].velocity.Set(0, 0, -1000);
	particulas[0].mass = 1000000;
	particulas[0].radius = 10;
	*/
	/*PLANETAS ORBITANDO
	//test:
	particulas[0].mass=1000000000;
	particulas[0].radius = 20;
	particulas[0].position.Set(0, 0, 0);
	
	particulas[1].position.Set(200, 0, 0);
	particulas[1].velocity.Set(0, 0, 150);
	particulas[1].mass = 10000;
	particulas[1].radius = 5;

	particulas[2].position.Set(0, 0, 100);
	particulas[2].velocity.Set(220, 0, 0);
	particulas[2].mass = 100;
	particulas[2].radius= 3;

	particulas[3].position.Set(0, 100, 100);
	particulas[3].velocity.Set(0, -300, -100);
	particulas[3].mass = 1000;
	particulas[3].radius = 1;

	particulas[4].position.Set(100, 100, 100);
	particulas[4].velocity.Set(0, -0, -100);
	particulas[4].mass = 100000;
	particulas[4].radius = 1;
	*/

	/*
	particulas[5].position.Set(0, 200, 000);
	particulas[5].velocity.Set(0, -100, 0);
	particulas[5].mass = 1000;
	particulas[5].radius = 4;


	particulas[6].position.Set(0, 100, 0);
	particulas[6].velocity.Set(0, 200, 0);
	particulas[6].mass = 10000;
	particulas[6].radius = 8;
	*/

	CopyParticles(estadoParticulas, particulas, NumParticulas);
	CopyParticles(estadoEstrellas, estrellas, NumEstrellas);
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

//TODO ESTO DEBERÍA ESTAR EN ESCENA

//Valores necesarios para el display
const double deltat = 1000;			
const double G = 6.67384*0.00080;//Constante que usas en la gravitacion universal

//Controlan el tamaño de las lineas verdes y rojas de debug
//Cuánto más grande sea, más pequeñas son las lineas
float velocityDebug = -7;
float accelerationDebug = -7;

//Dibuja
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//Reajustamos la pos de la luz
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glPushMatrix();


	//Pone la rotación de la escena correspondiente
	glTranslatef(-shor, sheight, -sdepth);
	glRotatef(-stheta, 1, 0, 0);
	glRotatef(sphi, 0, 1, 0);

	/*
	switch (contEscena)
	{
	case 0: test1.dibuja(); break;
	case 1: testRB1.dibuja(); break;

	default:
	break;

	}*/

	//Si pulsas la flecha derecha
	if (rightArrow)
	{
		for (int i = 0; i < NumParticulas; i++)
			UniversalGravitation(i, particulas.data(), NumParticulas, estadoParticulas[i], 1.0 / deltat, G);
		
	}

	//Si pulsas la flecha izquierda
	if (leftArrow)
	{
		for (int i = 0; i < NumParticulas; i++)//Calcula la gravedad direccional de cada una de las particulas
			DirectionalGravity(i, particulas.data(), NumParticulas, estadoParticulas[i], 1.0 / deltat);
		
	}
	//Resuelve las colisiones que pudieran haber
	CollisionResolution(NumParticulas, particulas.data(), estadoParticulas.data());

	//Copia el estado
	CopyStates(estadoParticulas.data(), particulas.data(), NumParticulas);


	//Dibuja las particulas
	for (int i = 0; i<NumParticulas; i++)
		particulas[i].dibuja();

	//A LAS ESTRELLAS Y AL DEBUG DEL RECORRIDO NO LES AFECTA LA LUZ
	glDisable(GL_LIGHTING);
	
	//---------------ESTRELLAS---------------

	for (int i = 0; i<NumEstrellas; i++)
	{
		SetColor(WHITE);
		glBegin(GL_POINTS);

		glVertex3f(estrellas[i].position.x, estrellas[i].position.y, estrellas[i].position.z);
		glEnd();	
	}


	//---------------ESTRELLAS---------------

	//COMENTAR ESTO SI NO SE QUIERE EL RECORRIDO DE DEBUG
	//---------------RECORRIDO PARTICULAS---------------

	//Borra los puntos de trayectoria si hay demasiados
	for (int i = 0; i<NumParticulas; i++)
	{
		if (posParticulas[i].size()>2600000)
			posParticulas[i].erase(posParticulas[i].begin(), posParticulas[i].begin() + 1);
	}

	//Crea un nuevo punto de la trayectoria y lo añade al vector
	for (int i = 0; i<NumParticulas; i++)
		posParticulas[i].push_back(Vector3d(particulas[i].position.x, particulas[i].position.y, particulas[i].position.z));

	//Dibuja todos los puntos de la trayectoria para cada particula
	for (int i = 0; i<NumParticulas; i++)
	{
		glPointSize(1);
		SetColor(WHITE);
		glBegin(GL_POINTS);

		for (int j = 1; j<posParticulas[i].size(); j++)
			glVertex3f(posParticulas[i][j].x, posParticulas[i][j].y, posParticulas[i][j].z);
		
		glEnd();
	}
	glEnable(GL_LIGHTING);

	//---------------RECORRIDO PARTICULAS---------------

	//COMENTAR ESTO SI NO SE QUIEREN LINEAS DE DEBUG
	//-------------VECTOR DEBUG----------------
	
	//Draw velocity and acceleration vectors
	SetColor(GREEN);
	for (int i = 0; i<NumParticulas; i++)
		DrawVector3d(particulas[i].velocity*pow(1.89, velocityDebug), particulas[i].position.x, particulas[i].position.y, particulas[i].position.z);

	SetColor(RED);
	for (int i = 0; i<NumParticulas; i++)
		DrawVector3d(particulas[i].acceleration*pow(3.1, accelerationDebug), particulas[i].position.x, particulas[i].position.y, particulas[i].position.z);
		
	dibujaEjes();

	//-------------VECTOR DEBUG----------------

	glPopMatrix();

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

