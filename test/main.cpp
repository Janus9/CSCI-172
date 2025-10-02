#include <string.h> // For string use
#ifdef __APPLE__
#include <GLUT/glut.h> // GLUT Header
#else
#include <GL/glut.h> // GLUT Header
#endif

#include <stdlib.h> // For rand() and exit()
/* GLUT callback Handlers */

float x1,y1,x,y,a,b,c,d,e,f; // Vars for Fractal Fern transform
int n = 200000; // Iteration count for Fractal loop
int num; // Random Selector

// Handles resizing the window without cutting out image (centers it w/ scaling)
void resize(int width, int height)
{
    // glViewport defines which part of window openGL will actually draw
    if(width<=height)
        // If window is taller than it is wide
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        // If window is wider than it is tall
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    x = 0;
    y = 0;
    // Functions used so openGL knows how to squash and resize and image
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
  glColor3f(0,1,0); // glColor3f takes 3 colors in float values (0-1) instead of 0-255. So this is green.
  for(int i = 0; i<n; i++) {
    num = rand()%100; // Random # 0-100
    // 1%, 7%, 7%, 85% chance distribution
    if (num == 0) { // f1 stem transform
        a = 0; b = 0; c = 0; d = 0.16; e = 0; f = 0;
    } else if (num <= 7) { // f3 leaf transform
        a = 0.2; b = 0.23; c = -0.26; d = 0.22; e = 0; f = 1.6;
    } else if (num <= 14) { // f4 other leaf transform
        a = -0.15; b = 0.26; c = 0.28; d = 0.24; e = 0; f = 0.44;
    } else { // f2 main transform
        a = 0.85; b = -0.04; c = 0.04; d = 0.85; e = 0; f = 1.6;
    }
    // Transform function
    x1 = a*x + c*y + e;
    y1 = b*x + d*y + f;
    // Move start to result of transform
    x = x1;
    y = y1;
    glBegin(GL_POINTS); // Emit one point
    glVertex3f(x1, y1, 1); // One point emitted at x1,y1
    glEnd();

  }
  glFlush ();                           // clear buffer
}

// runs with keypress in separate thread
void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }
    glutPostRedisplay(); // have glut redraw (incase keypress changes display state)
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv); // glut initilization
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // glut setup for rgb display
   glutInitWindowSize (800, 600);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop

   return EXIT_SUCCESS;
}
