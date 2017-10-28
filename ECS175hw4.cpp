#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <glaux.h> //Texture

using namespace std;

/* Global variables */
bool key[256];         //Save keyboard array
GLuint Triangle,rtri;
GLuint Cube;
void f(int a);
void Car();

/* Textures */
int LoadGLTextures();
AUX_RGBImageRec *LoadBMP(char *Filename);
GLuint texture[1];

/*  Set initial size of the display window.  */
GLsizei winWidth = 800, winHeight = 400;  

/* Lighting */
BOOL light;
GLfloat LightAmbient[]= {0.5, 0.5, 0.5, 1.0};  //Ambient Light
GLfloat LightDiffuse[]= {1.0, 1.0, 1.0, 1.0};  //Diffuse Light, Max soft white light
GLfloat LightPosition[]={0.0,0.0,2.0,1.0};     //Light Position

/* Control */
GLfloat angle;
GLfloat xrot;       //x rotate
GLfloat yrot;       //y rotate
GLfloat xspeed;     //x rotate speed
GLfloat yspeed;     //y rotate speed
GLfloat z = -0.5;   // depth into 

int init() 
{
	if(!LoadGLTextures())
		return FALSE;
	glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black and opaque
	glClearDepth(1.0);                // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);          // Enable depth testing for z-culling
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);           // Set the type of depth-test
    glShadeModel(GL_SMOOTH);          // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);           // Enable lighting
	return TRUE;
}
 
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glPushMatrix();
	glTranslatef(-2.0, 0.0, 0.0);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(angle,0.0, 1.0, 0.0);
	//glRotatef(xrot,1.0, 0.0, 0.0);
	//glRotatef(yrot,0.0, 1.0, 0.0);

	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glCallList(Triangle);
	glPopMatrix();

	glPushMatrix();
	gluLookAt(-4.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(1.5,0.0,z);
	glRotatef(xrot,1.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glCallList(Cube);
	glPopMatrix();

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
void reshape(GLsizei width, GLsizei height)
{ 
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION); 
   glLoadIdentity();            
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
 
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
   glutInitWindowSize (winWidth, winHeight);  // Set the window's initial width & height
   glutInitWindowPosition(550, 50); // Position the window's initial top-left corner
   glutCreateWindow ("ECS175 Computer Graphic");         // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   init();  
   Car();
   glutTimerFunc(25, f, 0);// Our own OpenGL initialization
   glutMainLoop();  // Enter the infinite event-processing loop
   return 0;
}

void f(int a)
{ 
	glutTimerFunc(25, f, 0);
	
	angle += 1;
	if (angle > 360)
	{
		angle -= 360;
	}
	xrot+=1;
	if (xrot > 360)
	{
		xrot -= 360;
	}
	yrot+=1;
	if (yrot > 360)
	{
		yrot -= 360;
	}
	if (GLUT_KEY_UP)		
	{
		cout<<"-"<<endl;
		z-=0.02;		
	}
	if (GLUT_KEY_DOWN)		
	{
		cout<<"+"<<endl;
		z+=0.02;		
	}
	glutPostRedisplay();
}

void Car()
{
   Triangle = glGenLists(1);
   glNewList(Triangle, GL_COMPILE);
   glBegin(GL_TRIANGLES);	
		glNormal3f(1.0,0.0,0.0);
		//glColor3f (0.0, 1.0, 0.0);   //Right, GREEN
		glTexCoord2f(0.5, 1.0); glVertex3f( 0.0, 1.0, 0.0);	
		//glColor3f (0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0,-1.0, 1.0);
		//glColor3f (1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.0,-1.0, -1.0);	
		
		glNormal3f(-1.0,0.0,0.0);
		//glColor3f (0.0, 1.0, 0.0);   //Left, PINK
		glTexCoord2f(0.5, 1.0); glVertex3f( 0.0, 1.0, 0.0);	
		//glColor3f (1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,-1.0, -1.0);
		//glColor3f (0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,-1.0, 1.0);	

		glNormal3f(0.0,0.0,1.0);
		//glColor3f (0.0, 1.0, 0.0);  //Front, BLUE
		glTexCoord2f(0.5, 1.0); glVertex3f( 0.0, 1.0, 0.0);	
		//glColor3f (0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,-1.0, 1.0);
		//glColor3f (0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f( 1.0,-1.0, 1.0);	

		glNormal3f(0.0,0.0,-1.0);
		//glColor3f (0.0, 1.0, 0.0);   //Back, RED
		glTexCoord2f(0.5, 1.0); glVertex3f( 0.0, 1.0, 0.0);	
		//glColor3f (1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0,-1.0, -1.0);
		//glColor3f (1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,-1.0, -1.0);
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3f(0.0,-1.0,0.0);   //Bottom
		//glColor3f (0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0,-1.0, 1.0);	
		//glColor3f (0.0, 0.0, 1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0,-1.0, 1.0);	
		//glColor3f (1.0, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0,-1.0, -1.0);
		//glColor3f (1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.0,-1.0, -1.0);
	glEnd();
	glEndList();

	Cube = glGenLists(1);
    glNewList(Cube, GL_COMPILE);
	glBegin(GL_QUADS);
		glNormal3f( 0.0, 0.0, 1.0);					// Front
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);	
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);	
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0,  1.0);	
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0,  1.0);

		glNormal3f( 0.0, 0.0,-1.0);					// Back
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);	
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);	
		glTexCoord2f(0.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);	
		glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);	

		glNormal3f( 0.0, 1.0, 0.0);					// Top
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);	
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,  1.0,  1.0);	
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,  1.0,  1.0);	
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);	

		glNormal3f( 0.0,-1.0, 0.0);					// bottom
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);	
		glTexCoord2f(0.0, 1.0); glVertex3f( 1.0, -1.0, -1.0);	
		glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);	
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);	

		glNormal3f( 1.0, 0.0, 0.0);					// Right
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);	
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);	
		glTexCoord2f(0.0, 1.0); glVertex3f( 1.0,  1.0,  1.0);	
		glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);	

		glNormal3f(-1.0, 0.0, 0.0);					// Left
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);	
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);	
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0,  1.0,  1.0);	
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);	
	glEnd();
   glEndList();
}

AUX_RGBImageRec *LoadBMP(char *Filename)
{
	ifstream fin;
	if (!Filename)
		return NULL;
	
	fin.open(Filename, ios::in);
	if(fin)
		return auxDIBImageLoad(Filename);
	return NULL;
}

int LoadGLTextures()
{
	int Status = FALSE;
	AUX_RGBImageRec *TextureImage[2];
	memset(TextureImage,0,sizeof(void *)*1);
	if(TextureImage[0] = LoadBMP("6.bmp"))
	{
		Status = TRUE;
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
	if(TextureImage[1] = LoadBMP("5.bmp"))
	{
		Status = TRUE;
		glGenTextures(1, &texture[1]);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
	}
	if(TextureImage[0])
	{
		if(TextureImage[0]->data)
			free(TextureImage[0]->data);
		free(TextureImage[0]);
	}
	if(TextureImage[1])
	{
		if(TextureImage[1]->data)
			free(TextureImage[1]->data);
		free(TextureImage[1]);
	}
	return Status;
}
