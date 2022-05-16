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

void display() {
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

	glutSwapBuffers();
}

void resize(int w, int h) {
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

void keyBoardListener(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}

void specialKeyBoardListener(int key, int x, int y) {
	float fraction = 0.1f;
	switch (key) {
		case GLUT_KEY_UP:
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * fraction;
			z -= lz * fraction;
			break;
		case GLUT_KEY_LEFT:
			angle += 0.1f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT:
			angle -= 0.1f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
	}
}

int main(int argc, char** argv) {
	//init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(300, 300);
	glutCreateWindow("snowman");

	//callback
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(display);

	glutKeyboardFunc(keyBoardListener);
	glutSpecialFunc(specialKeyBoardListener);

	glEnable(GL_DEPTH_TEST);

	//loop
	glutMainLoop();

	return 0;
}