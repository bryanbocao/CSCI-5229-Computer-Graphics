/*

 *  HW3: Scene in 3D
 *  CSCI-5229 Fall 2016 Computer Graphics
 *  Instructor: Willem Schreuder
 *  Student: Bo Cao
 *  Email: boca7588@colorado.edu or bo.cao-1@colorado.edu
 *  
 *  3D Objects
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  m/M        Cycle through different sets of objects
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int axes=1;       //  Display axes

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
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

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 *
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}
*/

/*
 *  Draw a cube with color 0 ~ 255 from (r, g, b)
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated psi about the x axis
 *     rotated theta about the y axis
 *     rotated phi about the z axis
 */
static void cube_color(double x,double y,double z,
                 double dx,double dy,double dz,
                 double r, double g, double b,
                 double phi, double theta, double psi)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(phi,1,0,0);
   glRotated(theta,0,1,0);
   glRotated(psi,0,0,1);
   glScaled(dx,dy,dz);
   //scale color into 255 color space
   double sr = r / 255, sg = g / 255, sb = b / 255;
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3d(sr, sg, sb);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3d(sr, sg, sb);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3d(sr, sg, sb);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3d(sr, sg, sb);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3d(sr, sg, sb);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3d(sr, sg, sb);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}


/*
 *  Draw vertex in polar coordinates with specific color (r, g, b);
 *
static void Vertex_color(double r, double g, double b)
{
   double sr = r / 255, sg = g / 255, sb = b / 255;
   glColor3d(sr, sg , sb);
   glVertex3d(sr , sg , sb);
}
*/

/*
 *  Draw a sphere
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw a Megaman
 *     at (x, y, z)
 *     scale (ds)
 *     rotated phi about the x axis
 *     rotated theta about the y axis
 *     rotated psi about the z axis
 */ 
static void megaman(double x, double y, double z, double ds, double phi, double theta, double psi)
{
    // Save transformation
    glPushMatrix();

    // Offset and scale
    glTranslated(x, y, z);
    glRotated(phi, 1, 0, 0);
    glRotated(theta, 0, 1, 0);
    glRotated(psi, 0, 0, 1);
    glScaled(ds, ds, ds);

    // Head
    //cube_color(0, 0, 0, 1, 1, 1, 51, 102, 255, 0);

    // Draw Neck
    cube_color(0, 0.2, 0, 0.1, 0.05, 0.1, // x, y, z & dx, dy, dz
		102, 217, 255, 0, 0, 0); // r, g, b & phi, theta, psi    

    // Draw Body
    cube_color(0, 0, 0, 0.25, 0.2, 0.15, // x, y, z & dx, dy, dz
		102, 217, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    //sphere(0.3, 0.05, 0, 0.15);
    //sphere(-0.3, 0.05, 0, 0.15);
    cube_color(0, -0.27, 0, 0.19, 0.05, 0.1, // x, y, z & dx, dy, dz
		102, 217, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    
    // Draw Arm
    // Draw Left Arm
    cube_color(0.42, 0.05, 0, 0.18, 0.08, 0.08, // x, y, z & dx, dy, dz
		102, 217, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    cube_color(0.5, 0.05, 0, 0.05, 0.15, 0.15, // x, y, z & dx, dy, dz
		77, 148, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    cube_color(0.8, 0.05, 0, 0.25, 0.2, 0.2, // x, y, z & dx, dy, dz
		77, 148, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    cube_color(1.1, 0.05, 0, 0.05, 0.15, 0.15, // x, y, z & dx, dy, dz
		77, 148, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    sphere(1.1, 0.05, 0, 0.15);

    // Draw Right Arm
    cube_color(-0.32, 0.05, 0.16, 0.08, 0.08, 0.18, // x, y, z & dx, dy, dz
		102, 217, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    cube_color(-0.32, 0.05, 0.35, 0.15, 0.15, 0.05, // x, y, z & dx, dy, dz
		77, 148, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    cube_color(-0.32, 0.05, 0.65, 0.2, 0.2, 0.25, // x, y, z & dx, dy, dz
		77, 148, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    cube_color(-0.32, 0.05, 0.95, 0.15, 0.15, 0.05, // x, y, z & dx, dy, dz
		77, 148, 255, 0, 0, 0); // r, g, b & phi, theta, psi
    sphere(-0.32, 0.05, 0.95, 0.15);
    //cube_color(0.6, 0, 0, 0.4, 0., 0.2, 77, 148, 255, 0);

    // Draw Underwear
    cube_color(0, -0.4, 0, 0.15, 0.08, 0.12, // x, y, z & dx, dy, dz
		77, 148, 255, 0, 0, 0); // r, g, b & phi, theta, psi

    // Draw Legs
    // Draw Left Leg
    cube_color(0.15, -0.6, 0, 0.08, 0.17, 0.08, // x, y, z & dx, dy, dz
		102, 217, 255, 0, 0, 25); // r, g, b & phi, theta, psi
    cube_color(0.25, -0.8, 0, 0.14, 0.08, 0.14, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, 25); // r, g, b & phi, theta, psi
    cube_color(0.3, -0.9, 0, 0.2, 0.1, 0.2, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, 25); // r, g, b & phi, theta, psi
    cube_color(0.41, -1.1, 0, 0.3, 0.15, 0.3, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, 25); // r, g, b & phi, theta, psi
    cube_color(0.55, -1.4, 0, 0.4, 0.3, 0.4, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, 25); // r, g, b & phi, theta, psi
    cube_color(0.65, -1.6, 0.1, 0.5, 0.1, 0.5, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, 25); // r, g, b & phi, theta, psi

    // Draw Right Leg
    cube_color(-0.15, -0.6, 0, 0.08, 0.17, 0.08, // x, y, z & dx, dy, dz
		102, 217, 255, 0, 0, -25); // r, g, b & phi, theta, psi
    cube_color(-0.25, -0.8, 0, 0.14, 0.08, 0.14, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, -25); // r, g, b & phi, theta, psi
    cube_color(-0.3, -0.9, 0, 0.2, 0.1, 0.2, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, -25); // r, g, b & phi, theta, psi
    cube_color(-0.41, -1.1, 0, 0.3, 0.15, 0.3, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, -25); // r, g, b & phi, theta, psi
    cube_color(-0.55, -1.4, 0, 0.4, 0.3, 0.4, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, -25); // r, g, b & phi, theta, psi
    cube_color(-0.65, -1.6, 0.1, 0.5, 0.1, 0.5, // x, y, z & dx, dy, dz
		26, 117, 255, 0, 0, -25); // r, g, b & phi, theta, psi

    glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);

   //cube(3,3,3 , 0.3,0.3,0.3 , 0);
   //cube(5,0,0 , 0.2,0.2,0.2 , 45);
   //cube(0,5,0 , 0.4,0.4,0.2 , 90);
   megaman(0, 0, 0, 0.5, 0, 0, 0);
   //megaman(0, 3, 3, 2, 0, 0, 0);
   //megaman(2, 2, 2, 0.5, 0, 0, 0);
   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Print("Angle=%d,%d",th,ph);
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   const double dim=2.5;
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(1000,800);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Bo Cao CSCI-5229 Computer Graphics Assignment 3");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
