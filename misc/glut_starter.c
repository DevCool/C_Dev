/******************************************************
 *
 * GLUT test bed
 * by Philip R Simonson
 *
 ******************************************************
 */


#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float ratio;

/*********************************************************
void update(void);
void resizeDisplay(int width,int height);
void display(void);
void processKeys(unsigned char key,int x,int y);
void processSpecial(unsigned int key,int x,int y);
void processSpecialUp(unsigned int key,int x,int y);
*********************************************************/

void update(void) {
	/* update logic code here */
}

void resizeDisplay(int width,int height) {

	/* handle window resizing */
	if(height==0)
		height=1;
	ratio=1.0f*width/height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,width,height);
	gluPerspective(45,ratio,1,1000);

	glMatrixMode(GL_MODELVIEW);

}

void display(void) {
	/* basic draw code here */
	glClearColor(0.5f,0.5f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	gluLookAt(0.0f,0.0f,10.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
	glLoadIdentity();

	glRotatef(90.0f,1.0f,0.0f,0.0f);
	glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_QUADS);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
	glEnd();

	glPushMatrix();
	glColor3f(1.0f,0.5f,0.5f);
	glBegin(GL_TRIANGLES);
		glVertex3f(-2.0f,-2.0f,0.0f);
		glVertex3f(0.0f,-2.0f,0.0f);
		glVertex3f(-1.0f,2.0f,0.0f);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
}

void processKeys(unsigned char key,int x,int y) {
	/* process normal keys here */
	if(key==27&1) exit(0);
}

void processSpecial(int key,int x,int y) {
	/* special keys here */
}

void processSpecialUp(int key,int x,int y) {
	/* special keys pressed then released here */
}

int main(int argc,char*args[]) {

	glutInit(&argc,args);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,600);
	glutCreateWindow("GLUT Starter - by Philip R Simonson");

	glutDisplayFunc(&display);
	glutIdleFunc(&update);
	glutReshapeFunc(&resizeDisplay);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(&processKeys);
	glutSpecialFunc(&processSpecial);
	glutSpecialUpFunc(&processSpecialUp);

	glutMainLoop();

	return 1;

}

