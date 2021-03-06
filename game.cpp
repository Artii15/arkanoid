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
float cam_x = 0;
float cam_y = 0;
float cam_z = 25.0f;

//Ustawienia okna i rzutowania
int windowPositionX = 100;
int windowPositionY = 100;
float windowWidth = 800.0f;
float windowHeight = 600.0f; 
float prev_time = glutGet(GLUT_ELAPSED_TIME);
float dt = 0.0f;
Scene *scene;
bool game_started = false;

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
	float cur_time = glutGet(GLUT_ELAPSED_TIME);
	dt = (cur_time - prev_time)/1000.0f;
	prev_time = cur_time;
	glutPostRedisplay();
}

//Procedura rysująca
void displayFrame() {
	//Wyczyść bufor kolorów i bufor głębokości
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wylicz macierz rzutowania
	glm::mat4 p = glm::perspective(0.785f, windowWidth/windowHeight, 0.1f, 100.0f);
	//Wylicz macierz widoku
	glm::vec3 observer_position = glm::vec3(cam_x,cam_y,cam_z);
	glm::vec3 look_at_position = glm::vec3(0.0f,0.0f,0.0f);
	if(observer_position == look_at_position){
		observer_position[2] += 0.1f; // Zabezpieczenie przed ustawieniem obserwatora w punkcie na który patrzy
	}
	glm::mat4 v = glm::lookAt(observer_position,look_at_position,glm::vec3(0.0f,1.0f,0.0f)); 
	
	scene->run(v, p, game_started);
	
	//Tylny bufor na przedni
	glutSwapBuffers();
}

void keyDown(int c, int x, int y) {
	game_started = true;
	switch (c) {
	case GLUT_KEY_F1:
		cam_x = cam_x + 1.0f;
		break;
	case GLUT_KEY_F2:
		cam_x = cam_x - 1.0f;
		break;
	case GLUT_KEY_F3:
		cam_z = cam_z + 1.0f;
		break;
	case GLUT_KEY_F4:
		cam_z = cam_z - 1.0f;
		break;
	case GLUT_KEY_LEFT:
		scene->getBat()->setDirection(-10.0f);
		break;
	case GLUT_KEY_RIGHT:
		scene->getBat()->setDirection(10.0f);
		break;
	}
}

void keyUp(int c, int x, int y) {
	switch (c){
		case GLUT_KEY_LEFT:
			scene->getBat()->setDirection(0);
			break;
		case GLUT_KEY_RIGHT:
			scene->getBat()->setDirection(0);
			break;
	}	
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
	
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
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
	printf("Zamykanie\n");
	return 0;
}

// Nie zwalniać pamięci po inicjalizowanych tutaj obiektach pod warunkiem, że będą podpięte pod obiekt sceny
void initObjects(){
	// Światła
	struct light* lights[9];
	for(short i=0; i<9; i++){
		lights[i] = new struct light();
		lights[i]->k = 0.02f;
	}
	for(short i=0; i<9; i++){
		scene->addLight(lights[i]);
	}
	lights[0]->position = glm::vec4(-9.5f, 8.5f, -12.5f, 1);
	lights[1]->position = glm::vec4(9.5f, 8.5f, -12.5f, 1);
	lights[2]->position = glm::vec4(9.5f, 8.5f, 12.5f, 1);
	lights[3]->position = glm::vec4(-9.5f, 8.5f, 12.5f, 1);
	lights[4]->position = glm::vec4(-9.5f, -8.5f, -12.5f, 1);
	lights[5]->position = glm::vec4(9.5f, -8.5f, -12.5f, 1);
	lights[6]->position = glm::vec4(9.5f, -8.5f, 12.5f, 1);
	lights[7]->position = glm::vec4(-9.5f, -8.5f, 12.5f, 1);
	lights[8]->position = glm::vec4(0, 0, 0, 1);
	lights[8]->k = 0.001f;
	lights[8]->diffuse = glm::vec4(0.0f, 0.5f, 1.0f, 1);
	lights[8]->specular = glm::vec4(0.0f, 0.5f, 1.0f, 1);
	// Piłki
	for(int i=0; i<3; i++){
		Ball *b = new Ball();
		b->loadShaders("shaders/vertex/default.txt", "shaders/fragment/default.txt");
		b->loadObj("models/sphere.obj");
		b->setDiffuseTexture("textures/golf_ball.tga");
		b->setAmbientTexture(b->getTextures().diffuse, b->getSamplers().diffuse);
		b->setSpecularTexture(b->getTextures().diffuse, b->getSamplers().diffuse);
		b->setModelMatrix(glm::scale(b->getModelMatrix(), glm::vec3(0.5f, 0.5f, 0.5f)));
		scene->addBall(b);
	}
	// Paletka
	Bat *bat = new Bat();
	bat->loadShaders("shaders/vertex/default.txt", "shaders/fragment/default.txt");
	bat->loadObj("models/bat.obj");
	bat->setDiffuseTexture("textures/metal.tga");
	bat->setAmbientTexture(bat->getTextures().diffuse, bat->getSamplers().diffuse);
	bat->setSpecularTexture(bat->getTextures().diffuse, bat->getSamplers().diffuse);
	bat->setModelMatrix(glm::translate(glm::scale(bat->getModelMatrix(), glm::vec3(1.3f,2.0f,2.0f)), glm::vec3(0.0f, -5.0f, 0.0f)));
	scene->setBat(bat);
	// Pomieszczenie
	Drawable *box = new Drawable();
	box->loadShaders("shaders/vertex/default.txt", "shaders/fragment/default.txt");
	box->loadObj("models/box.obj");
	box->setDiffuseTexture("textures/t2.tga");
	box->setAmbientTexture(box->getTextures().diffuse, box->getSamplers().diffuse);
	box->setSpecularTexture(box->getTextures().diffuse, box->getSamplers().diffuse);
	scene->setBox(box);
	// Bloki do strącania: 3 rzędy po 6 bloków
	glm::mat4 m_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f)), glm::vec3(-2.9f,7.0f,0.0f));
	for(int i=0; i<3; i++){
		for(int j=0; j<6; j++){
			Block *b = new Block();
			b->loadShaders("shaders/vertex/default.txt", "shaders/fragment/default.txt");
			b->loadObj("models/cube.obj");
			b->setDiffuseTexture("textures/fire.tga");
			b->setAmbientTexture(b->getTextures().diffuse, b->getSamplers().diffuse);
			b->setSpecularTexture(b->getTextures().diffuse, b->getSamplers().diffuse);
			b->setModelMatrix(m_matrix);
			b->addCrackedTexturePath("textures/fire_cracked.tga");
			b->addCrackedTexturePath("textures/fire_cracked2.tga");
			scene->addBlock(b);
			m_matrix = glm::translate(m_matrix, glm::vec3(1.1f, 0, 0));
		}
		m_matrix = glm::translate(m_matrix, glm::vec3(-6*1.1f, -1.5f, 0));
	}
}
