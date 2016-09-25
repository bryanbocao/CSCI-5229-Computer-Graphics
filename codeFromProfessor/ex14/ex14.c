/*
 *  Texture Mapping
 *
 *  Demonstrates basic texture mapping.
 *
 *  Key bindings:
 *  t          Cycle through tecture maps
 *  c          Cycle through colors
 *  o          Cycle through objects
 *  m          Toggles texture mode between modulate and replace
 *  +/-        Change repeat
 *  x/X        Move corner left/right
 *  y/Y        Move corner up/down
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int mode=0;    //  Texture mode
int ntex=0;    //  Texture
int obj=0;     //  Base object
int color=0;   //  Color
int axes=1;    //  Display axes
int th=0;      //  Azimuth of view angle
int ph=0;      //  Elevation of view angle
double rep=1;  //  Repetition
double X=1;    //  Top Right X
double Y=1;    //  Top Right Y
unsigned int texture[3];  //  Texture names

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   int k;
   //  Length of axes
   const double len=2.0;
   //  Colors
   int R[] = {1,1,1,0,0,0,1,0};
   int G[] = {1,0,1,1,1,0,0,0};
   int B[] = {1,0,0,0,1,1,1,0};

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Set view angle
   glLoadIdentity();
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);

   //  Draw scene
   //  Select texture if textures are on
   if (ntex<0)
      glDisable(GL_TEXTURE_2D);
   else
   {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , mode?GL_REPLACE:GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[ntex]);
   }
   //  Draw object in selected color and with the above texture applied
   glColor3f(R[color],G[color],B[color]);
   switch (obj)
   {
      //  Square
      case 0:
         glBegin(GL_QUADS);
         glTexCoord2f(0.0,0.0); glVertex2f(-1,-1);
         glTexCoord2f(rep,0.0); glVertex2f(+1,-1);
         glTexCoord2f(rep,rep); glVertex2f( X, Y);
         glTexCoord2f(0.0,rep); glVertex2f(-1,+1);
         glEnd();
         break;
      //  Triangle
      case 1:
         glBegin(GL_TRIANGLES);
         glTexCoord2f(0.0  ,0.0); glVertex2f( -1,-1);
         glTexCoord2f(rep  ,0.0); glVertex2f( +1,-1);
         glTexCoord2f(rep/2,rep); glVertex2f(X-1, Y);
         glEnd();
         break;
      //  Circle or cone
      case 2:
      case 3:
         glBegin(GL_TRIANGLE_FAN);
         glTexCoord2f(0.5,0.5); glVertex3f(0,0,obj==3?1:0);
         for (k=0;k<=360;k+=10)
         {
            glTexCoord2f(rep/2*Cos(k)+0.5,rep/2*Sin(k)+0.5);
            glVertex2f(X*Cos(k),Y*Sin(k));
         }
         glEnd();
         break;
   }
   //  Switch off textures so it doesn't apply to the rest
   glDisable(GL_TEXTURE_2D);
   
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
   Print("Angle=%d,%d  Repeat=%.1f Texture=%s RGB=%d,%d,%d",
     th,ph,rep,mode?"Replace":"Modulate",R[color],G[color],B[color]);

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
   //  Reset view angle and corner
   else if (ch == '0')
   {
      th = ph = 0;
      X = Y = 1;
   }
   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Cycle texture
   else if (ch == 't' || ch == 'T')
      ntex = (ntex+2)%4-1;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Cycle colors
   else if (ch == 'c' || ch == 'C')
      color = (color+1)%8;
   //  Cycle objects
   else if (ch == 'o' || ch == 'O')
      obj = (obj+1)%4;
   //  X location
   else if (ch == 'x')
      X -= 0.1;
   else if (ch == 'X')
      X += 0.1;
   //  Y location
   else if (ch == 'y')
      Y -= 0.1;
   else if (ch == 'Y')
      Y += 0.1;
   //  Repetition
   else if (ch=='+')
      rep += 0.1;
   else if (ch=='-')
      rep -= 0.1;
   //  Keep values in range
   if (X<0.1) X = 0.1;
   if (Y<0.1) Y = 0.1;
   if (rep<1) rep = 1;
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
   glutCreateWindow("Textures");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Load textures
   texture[0] = LoadTexBMP("block.bmp");
   texture[1] = LoadTexBMP("crate.bmp");
   texture[2] = LoadTexBMP("water.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
