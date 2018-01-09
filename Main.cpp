#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <soil.h>




// angle of rotation for the camera direction
float angle = 0.0f;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float t = 0.0f;

void DrawCube(void);
GLfloat ctrlpointsCurve[4][3] = {
	{ 20.0, 0.0, 20.0 },{ 0.0, 0.0, 0.0 },
	{ -5.0, 0.0, -5.0 },{ -20.0, 0.0, -20.0 } }; 

GLfloat copy[4][3];
GLfloat pos[3];
GLfloat target[3];


GLuint textures[4];



int LoadTexture(char* tex)
{
	int id = SOIL_load_OGL_texture(tex, 4, 0, 0); //mipmaps viso4ina na rezolucia
	if (!id)
	{
		printf("texture %s not loaded", "logo");
	}
	return id;
}

 void deCastelju (int pointsCount,GLfloat points[][3], float t) {
	
	memcpy(copy, ctrlpointsCurve, sizeof(ctrlpointsCurve));
	for (int n = pointsCount - 2; n >= 0; n--) {
		for (int i = 0; i <= n; i++) {
			copy[i][0] = (1 - t)*copy[i][0] + t*copy[i + 1][0];
			copy[i][2] = (1 - t)*copy[i][2] + t*copy[i + 1][2];
		}
	}
	
}

void timer(int val)
{
	if (t <= 1.0f) {
		deCastelju(4, ctrlpointsCurve, t);
		//memcpy(&pos[0], &copy[0],3 * sizeof(copy[0]));
		pos[0] = copy[0][0];
		pos[1] = copy[0][1];
		pos[2] = copy[0][2];
		t += 0.0005f;
		deCastelju(4, ctrlpointsCurve, t);
		target[0] = copy[0][0];
		target[1] = copy[0][1];
		target[2] = copy[0][2];
		//memcpy(&target[0], &copy[0], 3 * sizeof(copy[0]));
	}
	glutTimerFunc(val, timer, val);
	glutPostRedisplay();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void renderScene(void) {

	if (deltaAngle)
		computeDir(deltaAngle);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	
	gluLookAt(pos[0], 1.0f, pos[2],
	          target[0]+lx, 1.0f, target[2]+lz,
			  0.0f, 1.0f, 0.0f);
		
		
	//2ro decastelju za center

	// GROUND
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-50.0f, 0.0f, -50.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-50.0f, 0.0f, 50.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(50.0f, 0.0f, 50.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(50.0f, 0.0f, -50.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	

	for (int i = -5; i < 0; i++)
		for (int j = 0; j < 5; j++) {
			glPushMatrix();
			glTranslatef(i*10.0, 0, j * 10.0);
			DrawCube();
			glPopMatrix();
		}
	glutSwapBuffers();
}


void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT: deltaAngle = -0.01f; break;
		case GLUT_KEY_RIGHT: deltaAngle = 0.01f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
	}
}

int main(int argc, char **argv) {

	
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 700);
	
	glutCreateWindow("Animation");
//	glGenTextures(4, textures);
	textures[0] = LoadTexture("street.jpg");
	textures[1] = LoadTexture("b1.jpg");
	
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

	glutSpecialFunc(pressKey);

	// here are the new entries
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(10, timer, 10);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

void DrawCube()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	// draw a cube (6 quadrilaterals)
	glEnable(GL_TEXTURE_2D);								// Front Face
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);				// start drawing the cube.
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-3.0f, -1.0f, 3.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.0f, -1.0f, 3.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(3.0f, 10.0f, 3.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 10.0f, 3.0f);	// Top Left Of The Texture and Quad
	glEnd();

	glBegin(GL_QUADS);				// start drawing the cube.// Back Face
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-3.0f, -1.0f, -3.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 10.0f, -3.0f);	// Top Right Of The Texture and Quad
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(3.0f, 10.0f, -3.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.0f, -1.0f, -3.0f);	// Bottom Left Of The Texture and Quad
	glEnd();
	
	
	glBegin(GL_QUADS);				// start drawing the cube.
	glVertex3f(-3.0f, 10.0f, -3.0f);	// Top Left Of The Texture and Quad
	glVertex3f(-3.0f, 10.0f, 3.0f);	// Bottom Left Of The Texture and Quad
	glVertex3f(3.0f, 10.0f, 3.0f);	// Bottom Right Of The Texture and Quad
	glVertex3f(3.0f, 10.0f, -3.0f);	// Top Right Of The Texture and Quad

																// Bottom Face
	
	glVertex3f(-3.0f, -1.0f, -3.0f);	// Top Right Of The Texture and Quad
	glVertex3f(3.0f, -1.0f, -3.0f);	// Top Left Of The Texture and Quad
	glVertex3f(3.0f, -1.0f, 3.0f);	// Bottom Left Of The Texture and Quad
	glVertex3f(-3.0f, -1.0f, 3.0f);	// Bottom Right Of The Texture and Quad
	glEnd();
	
	glBegin(GL_QUADS);				// start drawing the cube.
	// Right face
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(3.0f, -1.0f, -3.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(3.0f, 10.0f, -3.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(3.0f, 10.0f, 3.0f);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(3.0f, -1.0f, 3.0f);	// Bottom Left Of The Texture and Quad
	glDisable(GL_TEXTURE_2D);
	glEnd();

	glBegin(GL_QUADS);				// start drawing the cube.
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-3.0f, -1.0f, -3.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-3.0f, -1.0f, 3.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-3.0f, 10.0f, 3.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-3.0f, 10.0f, -3.0f);	// Top Left Of The Texture and Quad

	
	glEnd();					// Done Drawing The Cube
	
	glDisable(GL_TEXTURE_2D);
}