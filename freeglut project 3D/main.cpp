#include <stdio.h>
#include <math.h>
#include <string>
#include <time.h>
#include <GL/freeglut.h>

#include "Includes.h"
#include "Physics_Functions.h"
#include "Escena.h"
#include "EscenaOrbitas.h"
#include "TestParticles.h"
//--------------CAMARA-------------

//N Y F son 0 por el gluPerspective
GLdouble xRight = 0, xLeft = 800, yTop = 600, yBot = 0, N = 0, F = 0;
int WIDTH = 800, HEIGHT = 600;

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

Escena * escena;

TEscenas estado;

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
	estado = EParticles;
	escena = new TestParticles();
	//escena = new Escena(4000);
	//contEscena = 0;

}

//INIT DE OPENGL
//Igual que en IG excepto por la c�mara
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
	//Activa el modelo de iluminaci�n /disable
	glEnable(GL_LIGHTING);

	//Enciende una luz particular /disable
	glEnable(GL_LIGHT0);

	//Definir componentes difusa, especular y ambiente
	GLfloat d[] = { 0.7f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, d);//Luz que vuelve a irradiar la superficie en todas las direcciones.
	GLfloat a[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, a);//Luz que alcanza una superficie aunque no est� expuesta a la fuente de luz.
	GLfloat s[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, s);//Luz que refleja la superficie.
	//GLfloat p[] = { 25.0f, 25.0f, 25.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, p);//Define la posicion de una fuente de luz. ultimo par�metro: 1 local, 0 direccional
	
	//luz ambiente
	/*
	GLfloat amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	*/
	//--------------LUZ-------------

	//-------------CAMARA-----------
	/*
		Hacer el setup igual que en IG lo rompe por el movimiento con el rat�n,
		lo dejamos tal como est�
	*/
	//-------------CAMARA-----------

}


//Dibuja
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//Reajustamos la pos de la luz
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glPushMatrix();

	escena->dibuja();

	dibujaEjes();

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

//Se le llama con la pulsaci�n del rat�n
void mouse(int button, int state, int x, int y)
{
	escena->setDown(x, y);
	//downX = x; downY = y;
	leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));

	middleButton = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));

	rightButton = ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN));
}

//Se le llama si se mantiene una tecla (botones del raton)
//Movimiento de la camara 
void motion(int x, int y)
{
	//Rota la c�mara respecto al centro
	if (leftButton)
		escena->leftButtonMouse(x,y);
	
	//Aleja o acerca la c�mara
	if (rightButton)
		escena->rightButtonMouse(x, y);

	//Permite mover la c�mara en el plano
	if (middleButton)
		escena->middleButtonMouse(x, y);
	
	//downX = x;   downY = y;
	escena->setDown(x, y);

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

		//case '3': contEscena++; contEscena %= 2; break;//Rota la c�mara alrededor del ejeX

	//Permiten mover la altura de la luz
	case 'w': 
		lightPos[1] += 0.1;
		break;
	case 's':
		lightPos[1] -= 0.1;
		break;

		//z y x en cada una de las escenas se utiliza de forma diferente
	case 'z':
		escena->zSetActivate(!escena->getZActivate());
		break;

	case 'x':
		escena->xSetActivate(!escena->getXActivate());
		break;

	case '1':
		estado = EEspiral;
		delete escena;
		escena = new EscenaOrbitas(estado);
		leftArrow = rightArrow = false;
		break;

	case '2':
		estado = EPlanetas;
		delete escena;
		escena = new EscenaOrbitas(estado);
		leftArrow = rightArrow = false;
		break;

	case '3':
		estado = EColision;
		delete escena;
		escena = new EscenaOrbitas(estado);
		leftArrow = rightArrow = false;
		break;

	case '4':
		estado = EParticles;
		delete escena;
		escena = new TestParticles();
		leftArrow = rightArrow = false;
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

		//Baja la posici�n de la luz
	case GLUT_KEY_DOWN:

		break;

	case GLUT_KEY_RIGHT:
		rightArrow = !rightArrow;
		escena->rightArrowActivate(rightArrow);
		break;

	case GLUT_KEY_LEFT:
		leftArrow = !leftArrow;
		escena->leftArrowActivate(leftArrow);
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

