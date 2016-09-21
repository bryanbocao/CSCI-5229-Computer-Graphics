/*
 *  GPS trace
 *
 *  Displays a GPS position over a 24 hour period.
 *
 *  Key bindings:
 *  x/X        View down X-axis
 *  y/Y        View down Y-axis
 *  z/Z        View down Z-axis
 *  arrows     Change view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

//  Globals
int th=0;         // Azimuth of view angle
int ph=0;         // Elevation of view angle
int n=0;          // Number of data points
int l=0;          // Movement
typedef struct {double x,y,z;} xyz_t;
xyz_t* xyz=NULL;
xyz_t  ctr;

/*
 *  Display the scene
 */
void display()
{
   int k;
   double len=10;
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);
   glRotatef(-90,1,0,0);
   //  Set transformation
   glPushMatrix();
   glTranslated(-ctr.x,-ctr.y,-ctr.z);
   //  Draw trace in yellow
   glColor3f(1,1,0);
   glBegin(GL_LINE_STRIP);
   for (k=0;k<n;k++)
      glVertex3d(xyz[k].x,xyz[k].y,xyz[k].z);
   glEnd();
   //  Draw point in red (10 pixels)
   glColor3f(1,0,0);
   glPointSize(10);
   glBegin(GL_POINTS);
   glVertex3d(xyz[l].x,xyz[l].y,xyz[l].z);
   glEnd();
   //  Restore transformation
   glPopMatrix();
   //  Draw axes in white
   glColor3f(0,0,1);
   glBegin(GL_LINES);
   glVertex3d(-len, 0.0, 0.0);
   glVertex3d(+len, 0.0, 0.0);
   glVertex3d( 0.0,-len, 0.0);
   glVertex3d( 0.0,+len, 0.0);
   glVertex3d( 0.0, 0.0,-len);
   glVertex3d( 0.0, 0.0,+len);
   glEnd();
   //  Label axes
   glRasterPos3d(len,0.0,0.0);
   Print("X");
   glRasterPos3d(0.0,len,0.0);
   Print("Y");
   glRasterPos3d(0.0,0.0,len);
   Print("Z");
   //  Display parameters
   glColor3f(1,1,1);
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d",th,ph);
   //  Flush and swap
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
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
   else if (ch == 'x' || ch == 'X')
   {
      th = -90;
      ph = 0;
   }
   else if (ch == 'y' || ch == 'Y')
      th = ph = 0;
   else if (ch == 'z' || ch == 'Z')
   {
      th = 0;
      ph = 90;
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
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
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   double dim=20;
   //  Ratio of the width to the height of the window
   double asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(0,asp,dim);
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   l = glutGet(GLUT_ELAPSED_TIME) % n;
   glutPostRedisplay();
}

/*
 *  Read GPS data from file
 */
void ReadGPS(void)
{
   int k=0;       //  Counter
   double x,y,z;  //  Working coordinates
   xyz_t min,max; //  Minimum and maximum coordinates

   //  Open file
   FILE *f = fopen("gps.dat","r");
   if (!f) Fatal("Cannot open file gps.dat\n");
   //  Read coordinates
   for (n=0;fscanf(f,"%lf %lf %lf",&x,&y,&z)==3;n++)
   {
      if (n+1>k)
      {
         k += 32768;
         xyz = (xyz_t*)realloc(xyz,k*sizeof(xyz_t));
      }
      xyz[n].x = x;
      xyz[n].y = y;
      xyz[n].z = z;
   }
   fclose(f);
   //  Find coordinate range
   min = max = xyz[0];
   for (k=1;k<n;k++)
   {
      if (xyz[k].x < min.x) min.x = xyz[k].x;
      if (xyz[k].y < min.y) min.y = xyz[k].y;
      if (xyz[k].z < min.z) min.z = xyz[k].z;
      if (xyz[k].x > max.x) max.x = xyz[k].x;
      if (xyz[k].y > max.y) max.y = xyz[k].y;
      if (xyz[k].z > max.z) max.z = xyz[k].z;
   }
   //  Center
   ctr.x = (max.x+min.x)/2;
   ctr.y = (max.y+min.y)/2;
   ctr.z = (max.z+min.z)/2;
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   ReadGPS();
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("GPS Coordinates");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Tell GLUT to call "idle" when nothing else is going on
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   //  Return code
   return 0;
}
