#include "inc/Drawable.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "inc/Exception.h"
#include <vector>
#include "inc/Scene.h"
#include "inc/Ball.h"
#include "inc/Bat.h"
#include "inc/Block.h"
#include "inc/light.h"

using namespace std;

//Ustawienia okna i rzutowania
int windowPositionX = 100;
int windowPositionY = 100;
float windowWidth = 800.0f;
float windowHeight = 600.0f; 
Scene *scene;

//Inicjalizacja obiektów na scenie
void initObjects();

//Procedura wywoływana przy zmianie rozmiaru okna
void changeSize(int w, int h) {
	//Ustawienie wymiarow przestrzeni okna
	glViewport(0,0,w,h);
	//Zapamiętanie nowych wymiarów okna dla poprawnego wyliczania macierzy rzutowania
	windowWidth=w;
	windowHeight=h;
}

//Procedura uruchamiana okresowo. Robi animację.
void nextFrame(void) {
	glutPostRedisplay();
}

//Procedura rysująca
void displayFrame() {
	//Wyczyść bufor kolorów i bufor głębokości
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wylicz macierz rzutowania
	glm::mat4 p = glm::perspective(0.785f, windowWidth/windowHeight, 1.0f, 100.0f);
	//Wylicz macierz widoku
	glm::mat4 v = glm::lookAt(glm::vec3(0.0f,0.0f,20.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f)); 
	
	scene->run(v, p);
	
	//Tylny bufor na przedni
	glutSwapBuffers();
}

//Procedura inicjująca biblotekę glut
void initGLUT(int *argc, char** argv) {
	glutInit(argc,argv); //Zainicjuj bibliotekę GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //Alokuj bufory kolorów (podwójne buforowanie) i bufor kolorów
	
	glutInitWindowPosition(windowPositionX,windowPositionY); //Wskaż początkową pozycję okna
	glutInitWindowSize(windowWidth,windowHeight); //Wskaż początkowy rozmiar okna
	glutCreateWindow("Arkanoid 3D"); //Utwórz okno i nadaj mu tytuł
	
	glutReshapeFunc(changeSize); //Zarejestruj procedurę changeSize jako procedurę obsługującą zmianę rozmiaru okna
	glutDisplayFunc(displayFrame); //Zarejestruj procedurę displayFrame jako procedurę obsługująca odświerzanie okna
	glutIdleFunc(nextFrame); //Zarejestruj procedurę nextFrame jako procedurę wywoływanš najczęścięj jak się da (animacja)
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); // Dla zwalniania zasobów
}

int main(int argc, char** argv)
{
	initGLUT(&argc,argv);
	
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	scene = new Scene();
	initObjects();
	
	glutMainLoop();
	delete scene;
	printf("Zamykanie\n"); // Żeby wiedzieć czy włączyła się funkcja clean
	return 0;
}

void initObjects(){
	// Nie zwalniać pamięci po inicjalizowanych tutaj obiektach pod warunkiem, że będą podpięte pod obiekt sceny
	// Światła
	struct light *l1, *l2;
	l1 = new struct light();
	l1->position[2] = 5;
	l2 = new struct light();
	l2->position[0] = 5;
	scene->addLight(l1).addLight(l2);
	// Piłki
	for(int i=0; i<3; i++){
		Ball *b = new Ball();
		b->loadShaders("shaders/vertex/bat.txt", "shaders/fragment/bat.txt");
		b->loadObj("models/sphere.obj");
		b->setDiffuseTexture("textures/golf_ball.tga");
		b->setAmbientTexture(b->getTextures().diffuse, b->getSamplers().diffuse);
		b->setSpecularTexture(b->getTextures().diffuse, b->getSamplers().diffuse);
		scene->addBall(b);
	}
}
