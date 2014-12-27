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
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float x,z,lx,lz,angle,deltaAngle,deltaMove;
float ratio;

void computePos(float deltaMove) {
	x+=deltaMove*lx*0.1f;
	z+=deltaMove*lz*0.1f;
}

void computeDir(float deltaAngle) {
	angle+=deltaAngle;
	lx=sin(angle);
	lz=-cos(angle);
}

void updateMovement(void) {
	if(deltaMove) computePos(deltaMove);
	if(deltaAngle) computeDir(deltaAngle);
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

void drawSnowMan(void) {
	glColor3f(0.95f,0.95f,0.95f);

	// Draw Body
	glTranslatef(0.0f,0.75f,0.0f);
	glutSolidSphere(0.75f,20,20);

	// Draw Head
	glTranslatef(0.0f,1.0f,0.0f);
	glutSolidSphere(0.25f,20,20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f,0.10f,0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f,0.0f,0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f,0.45f,0.45f);
	glutSolidCone(0.08f,0.5f,10,2);
}

int i,j;

void display(void) {
	/* basic draw code here */
	glClearColor(0.5f,0.5f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x,1.0f,z,x+lx,1.0f,z+lz,0.0f,1.0f,0.0f);

	glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f,0.0f,-100.0f);
		glVertex3f(-100.0f,0.0f,100.0f);
		glVertex3f(100.0f,0.0f,100.0f);
		glVertex3f(100.0f,0.0f,-100.0f);
	glEnd();

	for(i=-3; i<3; i++) {
		for(j=-3; j<3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j*10.0);
			drawSnowMan();
			glPopMatrix();
		}
	}

	updateMovement();

	glutSwapBuffers();
}

void processKeys(unsigned char key,int x,int y) {
	/* process normal keys here */
	if(key==27) exit(0);
}

void processSpecial(int key,int x,int y) {
	/* special keys here */

	if(key==GLUT_KEY_UP) deltaMove=0.5f;
	else if(key==GLUT_KEY_DOWN) deltaMove=-0.5f;
	else if(key==GLUT_KEY_LEFT) deltaAngle=-0.05f;
	else if(key==GLUT_KEY_RIGHT) deltaAngle=0.05f;
}

void processSpecialUp(int key,int x,int y) {
	/* special keys pressed then released here */
	if(key==GLUT_KEY_UP) deltaMove=0.0f;
	else if(key==GLUT_KEY_DOWN) deltaMove=0.0f;
	else if(key==GLUT_KEY_LEFT) deltaAngle=0.0f;
	else if(key==GLUT_KEY_RIGHT) deltaAngle=0.0f;
}

int main(int argc,char*args[]) {

	glutInit(&argc,args);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	glutCreateWindow("GLUT Playground - by Philip R Simonson");

	x=0.0f;
	z=0.0f;
	lx=0.0f;
	lz=-1.0f;
	deltaAngle=0.0f;
	deltaMove=0.0f;
	angle=0.0f;
	ratio=0.0f;

	glutDisplayFunc(&display);
	glutIdleFunc(&display);
	glutReshapeFunc(&resizeDisplay);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(&processKeys);
	glutSpecialFunc(&processSpecial);
	glutSpecialUpFunc(&processSpecialUp);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;

}

