/*
JOSHUA BAYT
CSCI 172
• Use “a w d s “ keys to rotate the model
• Use arrow keys to rotate skybox
• Use “j k i l “ to move camera left, right, top, bottom and “o” to reset
• Use “ z, x” to zoom in and out the scene
• Use “space bar” to rest the whole scene 
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
#include <SOIL2.h>

#include <vector>

using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* -- STRUCTS -- */
// 3D vector
typedef struct{
          float x;
          float y;
          float z;
          } vec3;

// Load Texture Function
void TextureLoader(char *fileName, GLuint tex) {

     int width, height; // width & height for the image reader
     unsigned char* image;

     glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.

     image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    if(!image)cout<< "Image file "<<fileName<<" not Found "<<endl;
    // binding image data
     SOIL_free_image_data(image);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}

// Skybox Class
class SkyBox {
    public:
        GLuint tex[6]; // Texture IDs
        vec3 rotation = {0.0f, 0.0f, 0.0f};
        vec3 position = {0.0f, 0.0f, 5.0f};
        vec3 scale    = {30.0f, 5.0f, 30.0f};
        float xMin = 0.0f;
        float xMax = 1.0f;
        float yMin = 0.0f;
        float yMax = 1.0f;
        void setupTextures(char* fileNames[6]) {
            for (int i = 0; i < 6; i++) {
                glGenTextures(1, &tex[i]);
                TextureLoader(fileNames[i], tex[i]);
            }
        }
        void drawSkybox(bool enableNormals) {
            glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
            glDisable(GL_LIGHTING);
            glDisable(GL_CULL_FACE);
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();

            glColor3f(1.0,1.0,1.0);// set environment to white
            glScalef(scale.x,scale.y,scale.z);

            glRotatef(rotation.x, 1,0,0);
            glRotatef(rotation.y, 0,1,0);
            glRotatef(rotation.z, 0,0,1);

            // Front Wall

            glBindTexture(GL_TEXTURE_2D,tex[0]);
            if(enableNormals) {glNormal3f(0.0f, 0.0f, 1.0f);}
            glBegin(GL_QUADS);
                glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,1.0);
                glTexCoord2f(xMax,yMin); glVertex3f(1.0,1.0,1.0);
                glTexCoord2f(xMax,yMax); glVertex3f(1.0,-1.0,1.0);
                glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,1.0);
            glEnd();

            // Back Wall

            glBindTexture(GL_TEXTURE_2D,tex[1]);
            if(enableNormals) {glNormal3f(0.0f, 0.0f, -1.0f);}
            glBegin(GL_QUADS);
                glTexCoord2f(xMin,yMax); glVertex3f(1.0,-1.0,-1.0);
                glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,-1.0);
                glTexCoord2f(xMax,yMin); glVertex3f(-1.0, 1.0,-1.0);
                glTexCoord2f(xMin,yMin); glVertex3f(1.0,1.0,-1.0);
            glEnd();

            // Top Wall

            glBindTexture(GL_TEXTURE_2D,tex[2]);
            if(enableNormals) {glNormal3f(0.0f, 1.0f, 0.0f);}
            glBegin(GL_QUADS);
                glTexCoord2f(xMin,yMax); glVertex3f(1.0,1.0,-1.0);
                glTexCoord2f(xMin,yMin); glVertex3f(1.0,1.0,1.0);
                glTexCoord2f(xMax,yMin); glVertex3f(-1.0,1.0,1.0);
                glTexCoord2f(xMax,yMax); glVertex3f(-1.0,1.0,-1.0);
            glEnd();

            // Bottom Wall

            glBindTexture(GL_TEXTURE_2D,tex[3]);
            if(enableNormals) {glNormal3f(0.0f, -1.0f, 0.0f);}
            glBegin(GL_QUADS);
                glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,1.0);
                glTexCoord2f(xMin,yMax); glVertex3f(1.0,-1.0,1.0);
                glTexCoord2f(xMin,yMin); glVertex3f(1.0,-1.0,-1.0);
                glTexCoord2f(xMax,yMin); glVertex3f(-1.0,-1.0,-1.0);
            glEnd();

            // Right Wall

            glBindTexture(GL_TEXTURE_2D,tex[5]);
            if(enableNormals) {glNormal3f(1.0f, 0.0f, 0.0f);}
            glBegin(GL_QUADS);
                glTexCoord2f(xMax,yMin); glVertex3f(1.0,1.0,-1.0);
                glTexCoord2f(xMax,yMax); glVertex3f(1.0,-1.0,-1.0);
                glTexCoord2f(xMin,yMax); glVertex3f(1.0,-1.0,1.0);
                glTexCoord2f(xMin,yMin); glVertex3f(1.0,1.0,1.0);
            glEnd();

            // Left Wall

            glBindTexture(GL_TEXTURE_2D,tex[4]);
            if(enableNormals) {glNormal3f(-1.0f, 0.0f, 0.0f);}
            glBegin(GL_QUADS);
                glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,-1.0);
                glTexCoord2f(xMax,yMin); glVertex3f(-1.0,1.0,1.0);
                glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,1.0);
                glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,-1.0);
            glEnd();

            glPopMatrix();

            glEnable(GL_LIGHTING); 
        }
};

class Cube {
    public:
        vec3 rotation = {0.0f, 0.0f, 0.0f};
        vec3 position = {0.0f, 0.0f, 8.0f};
        vec3 scale    = {1.0f, 1.0f, 1.0f};
        GLuint tex;
        void setupTexture(char* fileName) {
            glGenTextures(1, &tex);
            TextureLoader(fileName, tex);
        }
        void drawCube() {
            glPushMatrix();
                glColor3f(1.0,0.0,0.0);// set cube to red
                glTranslatef(position.x,position.y,position.z);
                glScalef(scale.x,scale.y,scale.z);
                glBindTexture(GL_TEXTURE_2D,tex);
                glutSolidCube(1.0);
            glPopMatrix();
        }
};

/* -- GLOBALS -- */

SkyBox *skybox = new SkyBox();
SkyBox* cube = new SkyBox();
char* skyboxFiles[6] = {
    "images/front.jpg",
    "images/back.jpg",
    "images/top.jpg",
    "images/bottom.jpg",
    "images/right.jpg",
    "images/left.jpg"
};
char* cubeFiles[6] = {
    "images/chud.jpg",
    "images/chud.jpg",
    "images/chud.jpg",
    "images/chud.jpg",
    "images/chud.jpg",
    "images/chud.jpg"
};

vec3 cameraPos = {0.0f, 0.0f, 5.0f};

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    GLdouble Ratio = (double)width/(double)height;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,Ratio,0.1f, 19999.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        cameraPos.x,cameraPos.y,cameraPos.z,    // Camera position
        0.0,0.0,10.0,                           // Look at point   
        0.0,1.0,0.0                             // Up vector
    );

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glDepthMask(GL_FALSE); // Needed for true-skybox
    skybox->drawSkybox(false);
    glDepthMask(GL_TRUE); // Needed for fake-skybox
    cube->drawSkybox(true); // Fake skybox, we just reuse the code to draw a cube

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
        case 'w':
            cube->rotation.x += 5.0f;
            break;
        case 's':
            cube->rotation.x -= 5.0f;
            break;
        case 'a':
            cube->rotation.y -= 5.0f;
            break;
        case 'd':
            cube->rotation.y += 5.0f;
            break;
        case 'z':
            cameraPos.z += 1.0f;
            break;
        case 'x':
            cameraPos.z -= 1.0f;
            break;
        case 'j':
            cameraPos.x -= 1.0f;
            break;
        case 'l':
            cameraPos.x += 1.0f;
            break;
        case 'i':
            cameraPos.y += 1.0f;
            break;
        case 'k':
            cameraPos.y -= 1.0f;
            break;
        case ' ':
            cameraPos = {0.0f, 0.0f, 5.0f};
            cube->rotation = {0.0f, 0.0f, 0.0f};
            skybox->rotation = {0.0f, 0.0f, 0.0f};
            break;
        case 'q':
            exit(0);
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        skybox->rotation.x += 5.0f;
        break;
    case GLUT_KEY_DOWN:
        skybox->rotation.x -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        skybox->rotation.y += 5.0f;
        break;
    case GLUT_KEY_LEFT:
        skybox->rotation.y -= 5.0f;
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

    glEnable(GL_TEXTURE_2D);
    // Apply onto Texture

    skybox->setupTextures(skyboxFiles);
    cube->setupTextures(cubeFiles);
    cube->scale = {2.0f,2.0f,2.0f};
    cube->position = {0.0f,0.0f,5.0f};

}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

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