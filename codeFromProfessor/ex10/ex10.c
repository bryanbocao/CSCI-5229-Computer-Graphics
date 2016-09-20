/*
 *  Polygon Offset
 *
 *  Demonstrates the use of polygon offset to overdraw lines and polygons.
 *
 *  Key bindings:
 *  m          Toggle polygon offset on/off
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int mode=0;    //  Texture mode
int axes=1;    //  Display axes
int th=0;      //  Azimuth of view angle
int ph=0;      //  Elevation of view angle
const int n=1080;  //  Spiral degrees

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   int k;
   //  Length of axes
   const double len=1.2;

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Set view angle
   glLoadIdentity();
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);

   //  Enable Polygon Offset
   if (mode)
   {
     glEnable(GL_POLYGON_OFFSET_FILL);
     glPolygonOffset(1,1);
   }
   else
     glPolygonOffset(0,0);
   //  Draw yellow square
   glColor3f(1,1,0);
   glBegin(GL_QUADS);
   glVertex2f(-1.0,-1.0);
   glVertex2f(+1.0,-1.0);
   glVertex2f(+1.0,+1.0);
   glVertex2f(-1.0,+1.0);
   glEnd();
   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);
   //  Draw blue spiral
   glColor3f(0,0,1);
   glBegin(GL_LINE_STRIP);
   for (k=0;k<=n;k+=10)
      glVertex2f(k*Cos(k)/n,k*Sin(k)/n);
   glEnd();
   //  Draw red triangle
   glColor3f(1,0,0);
   glBegin(GL_TRIANGLES);
   glVertex2f(-0.1,-0.1);
   glVertex2f(+0.1,-0.1);
   glVertex2f( 0.0,+0.2);
   glEnd();
   
   //  Draw axes (white)
   glColor3f(1,1,1);
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

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Polygon Offset=%s",th,ph,mode ? "On" : "Off");

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
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
   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
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
   double dim=2;  //  Size of world
   //  Ratio of the width to the height of the window
   double asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Polygon Offset");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
