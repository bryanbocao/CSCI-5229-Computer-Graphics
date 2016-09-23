/*
 *  Colors
 *
 *  Demonstrate RGB, CMYK and HSV color spaces.
 *
 *  Key bindings:
 *  Up/Down     Select color component
 *  Right/Left  Increase/decrease color component
 */
#include "CSCIx229.h"
int mode=0;     //  What to change
double R,G,B;   //  Red/Green/Blue
double C,M,Y,K; //  Cyan/Magenta/Yellow/Black
double H,S,V;   //  Hue/Saturation/Value

// Three componenent minimum and maximum
#define MIN(a,b,c) ((a)<(b) &&  (a)<(c) ? (a) : ((b) < (c) ? (b) : (c)))
#define MAX(a,b,c) ((a)>(b) &&  (a)>(c) ? (a) : ((b) > (c) ? (b) : (c)))

/*
 *  Convert CMYK to RGB
 */
void CMYK2RGB(double C, double M, double Y, double K,double* R, double* G, double* B)
{
   *R = (1-K)*(1-C);
   *G = (1-K)*(1-M);
   *B = (1-K)*(1-Y);
}

/*
 *  Convert RGB to CMYK
 */
void RGB2CMYK(double R,double G,double B,double* C,double* M,double* Y,double* K)
{
   double max = MAX(R,G,B);
   *K = 1-max;
   if (max==0)
      *C = *M = *Y = 0;
   else
   {
      *C = (max-R)/max;
      *M = (max-G)/max;
      *Y = (max-B)/max;
   }
}

/*
 *  Convert RGB to HSV
 */
void RGB2HSV(double R,double G,double B,double* H,double* S,double* V)
{
   double min = MIN(R,G,B);
   double max = MAX(R,G,B);
   double del = max - min;

   *V = max;
   if (max==0 || del==0)
      *S = *H = 0;
   else
   {
      *S = del/max;
      if (R==max)
         *H = (0 + (G-B)/del)/6;  // between yellow & magenta
      else if (G==max)
         *H = (2 + (B-R)/del)/6;  // between cyan & yellow
      else
         *H = (4 + (R-G)/del)/6;  // between magenta & cyan
      if (*H<0) *H += 1;
   }
}

/*
 *  Convert HSV to RGB
 */
void HSV2RGB(double H, double S, double V, double* R, double* G, double* B)
{
   double h6 = H>=1 ? 0 : 6*H;
   int    i6 = h6;
   double f6 = h6-i6;
   double f1 = V*(1-S);
   double f2 = V*(1-S*f6);
   double f3 = V*(1-S*(1-f6));

   switch (i6)
   {
      case 0:  *R = V;  *G = f3; *B = f1; break;
      case 1:  *R = f2; *G = V;  *B = f1; break;
      case 2:  *R = f1; *G = V;  *B = f3; break;
      case 3:  *R = f1; *G = f2; *B = V;  break;
      case 4:  *R = f3; *G = f1; *B = V;  break;
      default: *R = V;  *G = f1; *B = f2; break;
   }
}

/*
 *  Display component bar
 */
void Bar(int k,double y,char* name,double val,double R,double G,double B)
{
   //  Print bar value
   if (mode==k)
     glColor3f(1,1,0);
   else
     glColor3f(1,1,1);
   glRasterPos2d(-0.7,y);
   Print("%s = %5.3f",name,val);
   //  Fill Bar
   glColor3f(R,G,B);
   glBegin(GL_QUADS);
   glVertex2d(   -0.3,y+0.02);
   glVertex2d(val-0.3,y+0.02);
   glVertex2d(val-0.3,y-0.02);
   glVertex2d(   -0.3,y-0.02);
   glEnd();
   //  Outline Bar
   glColor3f(1,1,1);
   glBegin(GL_LINE_LOOP);
   glVertex2d(-0.3,y+0.02);
   glVertex2d(+0.7,y+0.02);
   glVertex2d(+0.7,y-0.02);
   glVertex2d(-0.3,y-0.02);
   glEnd();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT);
   //  Undo previous transformations
   glLoadIdentity();
   //  Draw quad
   glColor3f(R,G,B);
   glBegin(GL_QUADS);
   glVertex2d(-0.8,0.8);
   glVertex2d(+0.8,0.8);
   glVertex2d(+0.8,0.0);
   glVertex2d(-0.8,0.0);
   glEnd();
   //  Outline quad
   glColor3f(1,1,1);
   glBegin(GL_LINE_LOOP);
   glVertex2d(-0.8,0.8);
   glVertex2d(+0.8,0.8);
   glVertex2d(+0.8,0.0);
   glVertex2d(-0.8,0.0);
   glEnd();
   //  RGB
   Bar(0,-0.125,"R",R,1,0,0);
   Bar(1,-0.200,"G",G,0,1,0);
   Bar(2,-0.275,"B",B,0,0,1);
   //  CMY
   Bar(3,-0.400,"C",C,0,1,1);
   Bar(4,-0.475,"M",M,1,0,1);
   Bar(5,-0.550,"Y",Y,1,1,0);
   Bar(6,-0.625,"K",K,1,1,1);
   //  HSV
   Bar(7,-0.750,"H",H,1,1,1);
   Bar(8,-0.825,"S",S,1,1,1);
   Bar(9,-0.900,"V",V,1,1,1);
   //  Render the scene
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
   if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT)
   {
      double d = (key == GLUT_KEY_RIGHT) ? +0.01 : -0.01;
      switch (mode)
      {
         case 0: R += d; if (R<0) R = 0; if (R>1) R = 1; break;
         case 1: G += d; if (G<0) G = 0; if (G>1) G = 1; break;
         case 2: B += d; if (B<0) B = 0; if (B>1) B = 1; break;
         case 3: C += d; if (C<0) C = 0; if (C>1) C = 1; break;
         case 4: M += d; if (M<0) M = 0; if (M>1) M = 1; break;
         case 5: Y += d; if (Y<0) Y = 0; if (Y>1) Y = 1; break;
         case 6: K += d; if (K<0) K = 0; if (K>1) K = 1; break;
         case 7: H += d; if (H<0) H = 0; if (H>1) H = 1; break;
         case 8: S += d; if (S<0) S = 0; if (S>1) S = 1; break;
         case 9: V += d; if (V<0) V = 0; if (V>1) V = 1; break;
      }
      switch (mode)
      {
         case 0: case 1: case 2:
            RGB2CMYK(R,G,B,&C,&M,&Y,&K);
            RGB2HSV(R,G,B,&H,&S,&V);
            break;
         case 3: case 4: case 5: case 6:
            CMYK2RGB(C,M,Y,K,&R,&G,&B);
            RGB2HSV(R,G,B,&H,&S,&V);
            break;
         case 7: case 8: case 9:
            HSV2RGB(H,S,V,&R,&G,&B);
            RGB2CMYK(R,G,B,&C,&M,&Y,&K);
            break;
      }
   }
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      mode = (mode+9)%10;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      mode = (mode+1)%10;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27) exit(0);
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(0,asp,1);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   R = G = B = 1;
   RGB2CMYK(R,G,B,&C,&M,&Y,&K);
   RGB2HSV(R,G,B,&H,&S,&V);
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Colors");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
