#include "inc/Drawable.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "inc/light.h"
#include "inc/Exception.h"
#include <vector>

using namespace std;

//Ustawienia okna i rzutowania
int windowPositionX = 100;
int windowPositionY = 100;
float windowWidth = 800.0f;
float windowHeight = 600.0f; 
std::vector<struct light*> lights;
Drawable *d;

void initLights(){
	lights.push_back(new light());
	lights[0]->position[2] = 5;
	
	lights.push_back(new light());
	lights[1]->position[0] = 5;
}

void clean(){
	for(unsigned int i=0; i<lights.size(); i++){
		if(lights[i] != NULL){
			delete lights[i];
		}
	}
	delete d;
}

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
	glm::mat4 v = glm::lookAt(glm::vec3(0.0f,0.0f,4.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f)); 
	
	d->draw(v, p, lights);
	
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
		
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	initLights();
	//////////// Ładowanie do pamięci karty //////////////////////
	d = new Drawable();
	d->loadShaders("shaders/vertex/bat.txt", "shaders/fragment/bat.txt");
	d->loadObj("models/cube.obj");
	d->setDiffuseTexture("textures/t2.tga");
	d->setAmbientTexture(d->getTextures().diffuse, d->getSamplers().diffuse);
	d->setSpecularTexture("textures/metal.tga");
	/////////////////////////////////////////////////////////////
	glutMainLoop();
	clean();
	
	return 0;
}
