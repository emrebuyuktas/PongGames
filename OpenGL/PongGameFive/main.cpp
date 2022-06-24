

#include <GL/freeglut_std.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>

#define WINDOW_W 800
#define WINDOW_H 500





#define WIDTH 1000
#define HEIGHT 600


static GLfloat screenLeft = -48;
static GLfloat screenRight = 48;
static const int FPS = 60;
static GLfloat stick1Position = -15.0/2.0;
static GLfloat stick2Position = -15.0/2.0;
static GLfloat stickThick = 1.0;
static GLfloat stickSize = 10;
static GLfloat moveXFactor = 0.5;
static GLfloat moveYFactor = 0.3;

static int playerOneScore = 0;
static int playerTwoScore = 0;



struct _ball
{
	GLfloat radius = 1;
	GLfloat X = 0.0;
	GLfloat Y = 0.0;
	int directionX = -1;
	int directionY = 1;
}ball;

void drawString(float x, float y, float z, const char* string) {
	glRasterPos3f(x, y, z);

	for (const char* c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Updates the position
	}
}


void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0 / aspect, 50.0 / aspect, -1.0, 1.0);
}

void draw_circle(float x, float y, float radius) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	static const int circle_points = 100;
	static const float angle = 2.0f * 3.1416f / circle_points;

	// this code (mostly) copied from question:
	glBegin(GL_POLYGON);
	double angle1 = 0.0;
	glVertex2d(radius * cos(0.0), radius * sin(0.0));
	int i;
	for (i = 0; i < circle_points; i++)
	{
		glVertex2d(radius * cos(angle1), radius * sin(angle1));
		angle1 += angle;
	}
	glEnd();
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRectf(screenLeft, stick1Position, screenLeft + stickThick, stick1Position + stickSize);
	glRectf(screenRight, stick2Position, screenRight + stickThick, stick2Position + stickSize);
	draw_circle(ball.X, ball.Y, ball.radius);
	std::string one = std::to_string(playerOneScore);
	std::string two = std::to_string(playerTwoScore);
	char const* pchar = one.c_str();
	char const* pchartwo = two.c_str();
	
	drawString(0.0, 27.0, 0.0, pchar);
	drawString(1.0, 27.0, 0.0, "-");
	drawString(2.0, 27.0, 0.0, pchartwo);

	glFlush();
	glutSwapBuffers();
}

void timer(int v)
{
	glutPostRedisplay();
	if (ball.X + ball.radius > 50)
	{
		playerOneScore += 1;
		ball.X = 0;
		ball.Y = 0;
		stick1Position = -15.0 / 2.0;
		stick2Position = -15.0 / 2.0;
	}
	else if (ball.X+ball.radius < -50)
	{
		playerTwoScore += 1;
		ball.X = 0;
		ball.Y = 0;
		stick1Position = -15.0 / 2.0;
		stick2Position = -15.0 / 2.0;
	}
	else {
		if (((stick1Position) < ball.Y) && (ball.Y < (stick1Position + stickSize)) && (ball.X - stickThick - ball.radius <=screenLeft))
		{
			ball.directionX = ball.directionX * (-1);
		}
		else if (((stick2Position) < ball.Y) && (ball.Y < (stick2Position + stickSize)) && (ball.X + stickThick + ball.radius >= screenRight))
		{
			ball.directionX = ball.directionX * (-1);
		}
		if (ball.Y > 29.5 || ball.Y < -29.5)
		{
			ball.directionY = ball.directionY * -1;
		}
	}


	ball.X = ball.X + (moveXFactor * ball.directionX);
	ball.Y = ball.Y + (moveYFactor * ball.directionY);
	glutTimerFunc(1000 / FPS, timer, v);
}


void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		if (stick1Position + stickSize < 30)
		{
			stick1Position = stick1Position + 0.7;
		}
		break;
	case 's':
		if (stick1Position > -30)
		{
			stick1Position = stick1Position - 0.7;
		}
		break;
	case 'k':
		if (stick2Position + stickSize < 30)
		{
			stick2Position = stick2Position + 0.7;
		}
		break;
	case 'm':
		if (stick2Position > -30)
		{
			stick2Position = stick2Position - 0.7;
		}
		break;
	}
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(260, 140);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Pong Game");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);
	//glutMouseFunc(mouse);
	//glutSpecialFunc(kbSpecial);
	glutKeyboardFunc(keyboardFunc);
	glutMainLoop();
}
