/*
 *  Visibility
 *
 *  Demonstrate hidden surface algorithms using cuboids.
 *
 *  Key bindings:
 *  m/M    Cycle hidden surface modes
 *  n/N    Cycle number of cubes
 *  r/R    Reverse directon on magenta vace
 *  a      Toggle axes
 *  arrows Change view angle
 *  0      Reset view angle
 *  ESC    Exit
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
int axes=1;       //  Display axes
int mode=0;       //  Depth mode
int rev=0;        //  Reverse bottom of cube
int n=1;          //  Number of cubes
char* text[] = {"Face Culling","Face Culling with Z-sort","Z-buffer","Z-buffer + Face Culling"};

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

//  Cubes
#define Nk 6
typedef struct {int id;double x,y,z , dx,dy,dz , th , Zp;} cube_t;
cube_t Cube[] = {
   {1,  0, 0, 0 , 0.3,0.3,0.3 ,  0 , 0},
   {2,  0, 1, 0 , 0.4,0.4,0.2 , 45 , 0},
   {3,  1, 0, 0 , 0.2,0.2,0.2 , 45 , 0},
   {4, -1, 0, 0 , 0.2,0.2,0.2 , 45 , 0},
   {5,  0, 0, 1 , 0.2,0.2,0.2 , 45 , 0},
   {6,  0, 0,-1 , 0.2,0.2,0.2 , 45 , 0},
   };

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
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Transform cube
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
   //  Counter clockwise
   if (rev)
   {
      glVertex3f(-1,-1,+1);
      glVertex3f(+1,-1,+1);
      glVertex3f(+1,-1,-1);
      glVertex3f(-1,-1,-1);
   }
   //  Clockwise
   else
   {
      glVertex3f(-1,-1,-1);
      glVertex3f(+1,-1,-1);
      glVertex3f(+1,-1,+1);
      glVertex3f(-1,-1,+1);
   }

   //  End
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

/*
 * Z component of projection
 */
double Zp(double x,double y,double z)
{
   return -Sin(th)*Cos(ph)*x + Sin(ph)*y + Cos(th)*Cos(ph)*z;
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   //  Undo previous transformations
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);

   //  Enable Z-buffering
   if (mode==2 || mode==3)
      glEnable(GL_DEPTH_TEST);
   else
      glDisable(GL_DEPTH_TEST);
   //  Enable face culling
   if (mode!=2)
      glEnable(GL_CULL_FACE);
   else
      glDisable(GL_CULL_FACE);

   //  Draw a single cube
   if (n==1)
      cube(0,0,0 , 1,1,1 , 0);
   //  Multiple cubes - fixed order
   else if (mode != 1)
   {
      int k;
      for (k=0;k<n;k++)
         cube(Cube[k].x,Cube[k].y,Cube[k].z , Cube[k].dx,Cube[k].dy,Cube[k].dz , Cube[k].th);
   }
   //  Multiple cubes - Z-sorted
   else
   {
      int i,k;
      cube_t* cubep[Nk];
      //  Calculate projection Z component
      for (k=0;k<n;k++)
         Cube[k].Zp = Zp(Cube[k].x,Cube[k].y,Cube[k].z);
      //  Initialize Z pointer array
      for (k=0;k<n;k++)
         cubep[k] = Cube+k;
      //  Sort cubes on Z ascending using bubble sort
      for (k=0;k<n-1;k++)
         for (i=k+1;i<n;i++)
            if (cubep[k]->Zp > cubep[i]->Zp)
            {
               cube_t* t = cubep[k];
               cubep[k]  = cubep[i];
               cubep[i]  = t;
            }
      //  Draw cubes
      for (k=0;k<n;k++)
         cube(cubep[k]->x,cubep[k]->y,cubep[k]->z , cubep[k]->dx,cubep[k]->dy,cubep[k]->dz , cubep[k]->th);
      //  Print sequence
      glColor3f(1,1,1);
      glWindowPos2i(5,25);
      Print("Z order");
      for (k=0;k<n;k++)
         Print(" %d=%.3f",cubep[k]->id,cubep[k]->Zp);
   }
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
   Print("Angle=%d,%d  Hidden=%s",th,ph,text[mode]);
   if (rev) Print(" Reverse");
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
   //  Toggle bottom CW/CCW
   else if (ch == 'r' || ch == 'R')
      rev = !rev;
   //  Toggle Z-buffering
   else if (ch == 'm' || ch == 'M')
      mode = (mode+1)%4;
   //  Switch number of cubes
   else if (ch == 'n' || ch == 'N')
      n = (n%Nk)+1;
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
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Visibility");
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
