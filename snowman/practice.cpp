// practice.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "glew.h"
#include "glut.h"
#include <iostream>
#include <math.h>
#pragma comment(lib,"glew32.lib")

float x = 0.0f, z = 5.0f;
float lx = 0.0f, lz = -1.0f;
float angle = 0.0f;

int xOrigin = -1;

float deltaAngle = 0.0f;
float deltaMove = 0;

#define RED 1
#define GREEN 2
#define BLUE 3
#define ORANGE 4

#define FILL 1
#define LINE 2

#define SHRINK 1
#define NORMAL 2

int fillMenu, shrinkMenu, mainMenu, colorMenu;

float red = 1.0f, blue = 0.5f, green = 0.5f;

float scale = 1.0f;

int menuFlag = 0;

int frame = 0;
int time = 0, timebase = 0;
char s[50];

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else if (state == GLUT_DOWN) {
			xOrigin = x;
		}
	}
}

void mouseMove(int x, int y) {
	if (xOrigin >= 0) {
		deltaAngle = (x - xOrigin) * 0.001f;

		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void drawSnowman() {
	glColor3f(1.0f, 1.0f, 1.0f);

	//body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	//head
	glTranslatef(0.0f,1.0f ,0.0f);
	glutSolidSphere(0.25f, 20, 20);

	//eye
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.1f, 0.03f, 0.23f);
		glutSolidSphere(0.03f, 10, 10);

		glTranslatef(-0.2f, 0.0f, 0.0f);
		glutSolidSphere(0.03f, 10, 10);
	glPopMatrix();

	//node
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.25f);
	glutSolidCone(0.05f, 0.05f, 10, 2);
}

void computePos(float deltaMove) {
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void renderScene() {
	if (deltaMove) {
		computePos(deltaMove);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f
	);

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	for (int i = -3; i < 3; i++) {
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
				glTranslatef(i * 10.0, 0, j * 10.0);
				drawSnowman();
			glPopMatrix();
		}
	}

	/*frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s, "Lighthouse3D - FPS:%4.2f",
			frame * 1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}*/

	glutSwapBuffers();
}

void changeSize(int w, int h) {
	if (h == 0) {
		h = 1;
	}
	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int x, int y) {
	glutSetMenu(mainMenu);

	switch (key) {
		case 27: 
			glutDestroyMenu(mainMenu);
			glutDestroyMenu(fillMenu);
			glutDestroyMenu(colorMenu);
			glutDestroyMenu(shrinkMenu);
			exit(0);
			break;
		case 's':
			if (!menuFlag) {
				glutChangeToSubMenu(2, "shrink", shrinkMenu);
			}
			break;
		case 'c':
			if (!menuFlag) {
				glutChangeToSubMenu(2, "Color", colorMenu);
			}
			break;
	}

	if (key == 27) {
		exit(0);
	}
}

void pressKey(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			deltaMove = 0.1f;
			break;
		case GLUT_KEY_DOWN:
			deltaMove = -0.1f;
			break;
		case GLUT_KEY_LEFT:
			deltaAngle = -0.01f;
			break;
		case GLUT_KEY_RIGHT:
			deltaAngle = 0.01f;
			break;
	}
}

void releaseKey(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
		case GLUT_KEY_UP: 
		case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void processMenuStatus(int status, int x, int y) {
	if (status == GLUT_MENU_IN_USE) {
		menuFlag = 1;
	}
	else {
		menuFlag = 0;
	}
}

void processMainMenu(int option) {

}

void processFillMenu(int option) {
	switch (option) {
	case FILL: glPolygonMode(GL_FRONT, GL_FILL);
	case LINE: glPolygonMode(GL_FRONT, GL_LINE);
	}
}

void processShrinkMenu(int option) {
	switch (option) {
	case SHRINK: scale = 0.5f; break;
	case NORMAL: scale = 1.0f; break;
	}
}

void processColorMenu(int option) {
	switch (option) {
		case RED:
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f; break;
		case GREEN:
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f; break;
		case BLUE:
			red = 0.0f;
			green = 0.0f;
			blue = 1.0f; break;
		case ORANGE:
			red = 1.0f;
			green = 0.5f;
			blue = 0.5f; break;
	}
}

void createPopupMenus() {
	shrinkMenu = glutCreateMenu(processShrinkMenu);

	glutAddMenuEntry("Shrink", SHRINK);
	glutAddMenuEntry("NORMAL", NORMAL);

	fillMenu = glutCreateMenu(processFillMenu);

	glutAddMenuEntry("Fill", FILL);
	glutAddMenuEntry("Line", LINE);

	colorMenu = glutCreateMenu(processColorMenu);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Orange", ORANGE);

	mainMenu = glutCreateMenu(processMainMenu);

	glutAddSubMenu("Polygon Mode", fillMenu);
	glutAddSubMenu("Color", colorMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMenuStatusFunc(processMenuStatus);
}



int main(int argc, char** argv) {
	//init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(300, 300);
	glutCreateWindow("snowman");

	//callback
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutIgnoreKeyRepeat(0);
	glutSpecialUpFunc(releaseKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	createPopupMenus();

	//loop
	glutMainLoop();


	return 0;
}