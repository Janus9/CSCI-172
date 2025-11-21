/**
 * /summary
 *  - Each face uses the structure: v/vt/vn
 *  - Thus each model contains a fixed amount of vertices, texture coordinates, and vertex normals
 *      - These are store in separate vectors<>
 */


#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
#include <vector>

#include <fstream>
#include <sstream>

using namespace std;

/* -- STRUCTS -- */

typedef struct {
    GLfloat x,y,z;
} Vec3f;

/* -- CLASSES -- */

class Camera {
    public:
        Vec3f position;
        Vec3f target;
        Vec3f up;
        bool enabled;
        void shoot() {
            gluLookAt(
                position.x, position.y, position.z,
                target.x, target.y, target.z,
                up.x, up.y, up.z);
            }
        void init() {
            // default camera values
            enabled = true;
            position = {0.0f, 0.0f, 5.0f};
            target   = {0.0f, 0.0f, 0.0f};
            up       = {0.0f, 1.0f, 0.0f};
        }      
};

Camera* mainCamera;
Camera* remoteCamera;

GLfloat teaPotTheta = 0.0f;
GLfloat mainCameraTheta = 0.0f;


/* GLOBAL VARIABLES */

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mainCamera->enabled) {
        mainCamera->shoot();
    } else {
        remoteCamera->shoot();
    }

    glPushMatrix();
        glTranslatef(0,0,0);
        glRotatef(teaPotTheta, 0.0f, 1.0f, 0.0f);
        glTranslatef(-mainCamera->position.x*2, -mainCamera->position.y*2, -mainCamera->position.z*2);
        glutSolidTeapot(1.0);
    glPopMatrix();
       
    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        // Exits program
        case 'q':
            exit(0);
            break;
        case '1':
            mainCamera->enabled = !mainCamera->enabled;
            break;
        case '2':
            teaPotTheta += 2.0f;
            break;
        case '3':
            mainCameraTheta += 0.1f;
            mainCamera->position.x = 10 * cos(mainCameraTheta);
            mainCamera->position.z = 10 * sin(mainCameraTheta);
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        break;
    case GLUT_KEY_DOWN:
        break;
    case GLUT_KEY_LEFT:
        break;
    case GLUT_KEY_RIGHT:
        break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    mainCamera = new Camera();
    mainCamera->init();
    mainCamera->position = {0.0f, 0.0f, -10.0f};
    remoteCamera = new Camera();
    remoteCamera->init();
    remoteCamera->position = {-5.0f, 10.0f, -8.0f};

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
