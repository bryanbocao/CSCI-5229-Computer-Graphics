/*
 * HW2: Lorenz Attractor
 * CSCI-5229 Fall 2016 Computer Graphics
 * Instructor: Willem Schreuder
 * Student: Bo Cao
 * Email: boca7588@colorado.edu or bo.cao-1@colorado.edu
 *
 * Simple program to demonstrate generating coordinates
 * using the Lorenz Attractor
 *
 * References are exercises in class
 * 
 */

#include <stdarg.h>
#include <stdio.h>

#define GL_GLEXT_PROTOTYPES

#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glut.h>

#endif

// Lorenz Parameters
double s;
double b;
double r;
int hasLorenz;

int hasAxes;
double lenOfAxes;

// view angles
int ph; //  Elevation of view angle
int th; //  Azimuth of view angle

void drawLorenz()
{
    glPushMatrix();
    glBegin(GL_LINE_STRIP);

    int i;
    /*  Coordinates  */
    double x = 1;
    double y = 1;
    double z = 1;
    /*  Time step  */
    double dt = 0.001;

    // printf("%5d %8.3f %8.3f %8.3f\n",0,x,y,z);
    /*
     *  Integrate 50,000 steps (50 time units with dt = 0.001)
     *  Explicit Euler integration
     */
    int steps = 50000;
    for (i=0;i<steps;i++)
    {
       double dx = s*(y-x);
       double dy = x*(r-z)-y;
       double dz = x*y - b*z;
       x += dt*dx;
       y += dt*dy;
       z += dt*dz;
       //printf("%5d %8.3f %8.3f %8.3f\n",i+1,x,y,z);
       double ratio = i / steps;
       glColor3d(i * i, i * i * i, 255 * ratio);
       glVertex3d(x, y, z);
    }

    glEnd();
    glPopMatrix();
}

#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(120, 180, 120,  // view point position
            0, 0, 0,        // view direction 
            0, 1, 0);

    glPushMatrix();

    // Set view angle
    glRotated(ph, 1, 0, 0);    // rotate to see from UP or DOWN
    glRotated(th, 0, 1, 0);    // rotate by th about y-axis
    
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Draw axes
    if (hasAxes)
    {
       glBegin(GL_LINES);

       glColor3d(255, 0, 0); //draw red x axis
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(lenOfAxes,0.0,0.0);
    
       glColor3d(0, 255, 0); //draw green y axis
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,lenOfAxes,0.0);

       glColor3d(0, 0, 255); //draw blue z axis
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,0.0,lenOfAxes);

       glEnd();

       glColor3d(255, 255, 255);
       //  Label axes
       glRasterPos3d(lenOfAxes,0.0,0.0);
       Print("X");
       glRasterPos3d(0.0,lenOfAxes,0.0);
       Print("Y");
       glRasterPos3d(0.0,0.0,lenOfAxes);
       Print("Z");
   }
   // end of drawing axes
   if (hasLorenz)
   {
       drawLorenz();
   }
   
   //  display parameters
   //  Five pixels from the lower left corner of the window
   glColor3d(255, 0, 255);
   glWindowPos2i(5,28);
   //  Print the text string
   Print("s=%f,  b=%f,  r=%f ",s,b,r);
   glColor3d(0, 255, 255);
   glWindowPos2i(5,5);
   Print("ph=%d,  th=%d",ph,th);

   glPopMatrix();

   glFlush();
   glutSwapBuffers();
}

void special(int key, int x, int y)
{
    if (key == GLUT_KEY_UP) {
        ph += 5;
    } else if (key == GLUT_KEY_DOWN) {
        ph -= 5;
    } else if (key == GLUT_KEY_RIGHT) {
        th += 5;
    } else if (key == GLUT_KEY_LEFT) {
        th -= 5;
    } 
    glutPostRedisplay();
}

void initParameters()
{
    // initialize lorenz parameters
    s  = 10;
    b  = 2.6666;
    r  = 28;
    hasLorenz = 1;

    // initialize view parameter
    hasAxes = 1;
    lenOfAxes = 100;
    ph = 0;
    th = 10;
}

void key(unsigned char ch, int x, int y)
{
    if (ch == 27) {
        exit(0);
    } else if (ch == ' ') {
        initParameters();
    } else if (ch == '0') {
        th = ph = 0;
    } else if (ch == 'S') {
	s++;
    } else if (ch == 's') {
    	s--;
    } else if (ch == 'B') {
	b++;
    } else if (ch == 'b') {
    	b--;
    } else if (ch == 'R') {
	r++;
    } else if (ch == 'r') {
    	r--;
    } else if (ch == 'a' || ch == 'A') {//  Toggle axes
        hasAxes = 1 - hasAxes;
    } else if (ch == 'l' || ch == 'L') {//  Toggle lorenz
        hasLorenz = 1 - hasLorenz;
    }
    glutPostRedisplay();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width, int height)
{
    //  Calculate width to height ratio
    double w2h = (height > 0) ? (double) width / height : 1;
    //  Set viewport as entire window
    glViewport(0, 0, width, height);
    //  Select projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Set projection to identity
    glLoadIdentity();
    //  Orthogonal projection:  unit cube adjusted for aspect ratio
    gluPerspective(60, w2h, 1, 10000);    // setup a perspective projection
    //  Select model view matrix
    glMatrixMode(GL_MODELVIEW);
    //  Set model view to identity
    glLoadIdentity();
}

void init()
{
    initParameters();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);      // enable hidden surface removal

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100, 1, 1, 20000);    // setup a perspective projection

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(120, 180, 120,  // view point position
            0, 0, 0,        // view direction 
            0, 1, 0);
}

int main(int argc, char *argv[])
{
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    //  Request 800 x 800 pixel window
    glutInitWindowSize(800, 800);
    //  Create the window
    glutCreateWindow("Bo Cao CSCI-5229 Computer Graphics Assignment 2");    // create the window
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);
    //initialise coordinates and lorenz parameters
    init();
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
}
