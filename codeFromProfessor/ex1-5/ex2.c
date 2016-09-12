/*
 *  Hello World Triangle Version 2
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
double th=0;  //  Rotation angle

/*
 * This function is called by GLUT to display the scene
 */
void display()
{
   //  Clear screen
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset transformations
   glLoadIdentity();
   //  Rotate around Y axis
   glRotatef(th,0.0,1.0,0.0);
   //  Multicolored triangle
   glBegin(GL_POLYGON);
   glColor3f(1.0,0.0,0.0);
   glVertex2f( 0.0, 0.5);
   glColor3f(0.0,1.0,0.0);
   glVertex2f( 0.5,-0.5);
   glColor3f(0.0,0.0,1.0);
   glVertex2f(-0.5,-0.5);
   glEnd();
   //  Flush and swap buffer
   glFlush();
   glutSwapBuffers();
}

/*
 * This function is called by GLUT when special keys are pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow - increase rotation by 5 degree
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Request display update
   glutPostRedisplay();
}

/*
 * GLUT based Hello World
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered true color window
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Create window
   glutCreateWindow("Hello World");
   //  Register display and key callbacks
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   //  Pass control to GLUT for events
   glutMainLoop();
   //  Return to OS
   return 0;
}
