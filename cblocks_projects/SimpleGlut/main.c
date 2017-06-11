/******************************************************
 *
 * GLUT test bed
 * by Philip R Simonson
 *
 ******************************************************
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct _tagBMPFILEHEADER {
    WORD filetype;
    DWORD filesize;
    DWORD reserved;
    DWORD offset;
    DWORD bytesInHeader;
    DWORD width;
    DWORD height;
    WORD planes;
    WORD bitsPerPixel;
    DWORD compression;
    DWORD size;
    DWORD horizRes;
    DWORD vertRes;
    DWORD indiciesUsed;
    DWORD indiciesImportant;
} BMPFILEHEADER, *LPBMPFILEHEADER;

unsigned int
read_bmp(filename, header)
const char *filename;
LPBMPFILEHEADER header;
{
    FILE *fin = fopen(filename, "rb");
    if(fin == NULL) {
        fprintf(stderr, "Cannot open file : %s\n", filename);
        return -1;
    }
    unsigned char *image = NULL;
    int width, height, rowsize;
    unsigned int tex;

    rewind(fin);
    memset(header, 0, sizeof(BMPFILEHEADER));
    fread(&header->filetype, 2, 1, fin);
    fread(&header->filesize, 4, 1, fin);
    fread(&header->reserved, 4, 1, fin);
    fread(&header->offset, 4, 1, fin);
    fread(&header->bytesInHeader, 4, 1, fin);
    fread(&header->width, 4, 1, fin);
    fread(&header->height, 4, 1, fin);
    fread(&header->planes, 2, 1, fin);
    fread(&header->bitsPerPixel, 2, 1, fin);
    fread(&header->compression, 4, 1, fin);
    fread(&header->size, 4, 1, fin);
    fread(&header->horizRes, 4, 1, fin);
    fread(&header->vertRes, 4, 1, fin);
    fread(&header->indiciesUsed, 4, 1, fin);
    fread(&header->indiciesImportant, 4, 1, fin);
    rewind(fin);
    width = header->width;
    height = header->height;
    if(width < 0)
        width *= -1;
    if(height < 0)
        width *= -1;

    rowsize = width*3;
    image = calloc(1, rowsize*height);
    fseek(fin, header->offset, SEEK_SET);
    int rc = fread(image, 3*width*height, 1, fin);
    fclose(fin);
/*    if (rc != header->filesize) {
        fprintf(stderr, "Image size not correct.\n");
        return 0;
    }
*/
    glGenTextures(1, &tex);
    glTexImage2D(GL_TEXTURE_2D, 0, header->bitsPerPixel, width, height, 0, GL_RGB, GL_UNSIGNED_INT, image);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 0);
    if(image != NULL) free(image);
    return tex;
}

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
static unsigned int texture1;
static BMPFILEHEADER texHeader1;

void display(void) {
	/* basic draw code here */
	glClearColor(0.5f,0.5f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x,1.0f,z,x+lx,1.0f,z+lz,0.0f,1.0f,0.0f);

	glColor3f(0.5f,0.5f,0.5f);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBegin(GL_QUADS);
        glTexCoord2d(1.0, 1.0);
		glVertex3f(-100.0f,0.0f,-100.0f);
		glTexCoord2d(1.0, 0.0);
		glVertex3f(-100.0f,0.0f,100.0f);
		glTexCoord2d(0.0, 0.0);
		glVertex3f(100.0f,0.0f,100.0f);
		glTexCoord2d(0.0, 1.0);
		glVertex3f(100.0f,0.0f,-100.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

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

	texture1 = read_bmp("testfile.bmp", &texHeader1);
	glutDisplayFunc(&display);
	glutIdleFunc(&display);
	glutReshapeFunc(&resizeDisplay);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(&processKeys);
	glutSpecialFunc(&processSpecial);
    glutSpecialUpFunc(&processSpecialUp);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	glDeleteTextures(1, &texture1);
	return 1;

}
