/**
 * /summary
 *  - Each face uses the structure: v/vt/vn
 *  - Thus each model contains a fixed amount of vertices, texture coordinates, and vertex normals
 *      - These are store in separate std::vectors<>
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
using namespace std;

/* CLASSES */

class Vertex3f {
    public:
        GLfloat x,y,z;
        Vertex3f() {
            x = 0.0f; y = 0.0f; z = 0.0f;
        }
        Vertex3f(GLfloat x, GLfloat y, GLfloat z) {
            this->x = x; this->y = y; this->z = z;
        }
};

class VertexTex2f {
    public:
        GLfloat u,v;
        VertexTex2f() {
            u = 0.0f; v = 0.0f;
        }
        VertexTex2f(GLfloat u, GLfloat v) {
            this->u = u; this->v = v;
        }
};

class Face3D {
    public:
        int v1, v2, v3;       // vertex indices
        int vt1, vt2, vt3;   // texture coordinate indices
        int vn1, vn2, vn3;   // vertex normal indices
};

class Model {
    public:
        vector<Vertex3f> vertices;
        vector<VertexTex2f> texCoords;
        vector<VertexNormal3f> normals;
        vector<Face3D> faces;
        void debugPrint() {
            for (int i = 0; i < vertices.size(); i++) {
                cout << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
            }
            for (int i = 0; i < texCoords.size(); i++) {
                cout << "vt " << texCoords[i].u << " " << texCoords[i].v << endl;
            }
            for (int i = 0; i < normals.size(); i++) {
                cout << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
            }
            for (int i = 0; i < faces.size(); i++) {
                cout << "f " << faces[i].v1 << "/" << faces[i].vt1 << "/" << faces[i].vn1 << " "
                             << faces[i].v2 << "/" << faces[i].vt2 << "/" << faces[i].vn2 << " "
                             << faces[i].v3 << "/" << faces[i].vt3 << "/" << faces[i].vn3 << endl;
            }
        }
};

class VertexNormal3f {
    public:
        GLfloat x,y,z;
        VertexNormal3f() {
            x = 0.0f; y = 0.0f; z = 0.0f;
        }
        VertexNormal3f(GLfloat x, GLfloat y, GLfloat z) {
            this->x = x; this->y = y; this->z = z;
        }
};

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

// Apply a single face of the model
void applyFace(Model *model, Face3D* face) {
    glVertex3f(model->vertices[face->v1].x, model->vertices[face->v1].y, model->vertices[face->v1].z); glTexCoord2f(model->texCoords[face->vt1].u, model->texCoords[face->vt1].v); glNormal3f(model->normals[face->vn1].x, model->normals[face->vn1].y, model->normals[face->vn1].z);
    glVertex3f(model->vertices[face->v2].x, model->vertices[face->v2].y, model->vertices[face->v2].z); glTexCoord2f(model->texCoords[face->vt2].u, model->texCoords[face->vt2].v); glNormal3f(model->normals[face->vn2].x, model->normals[face->vn2].y, model->normals[face->vn2].z);
    glVertex3f(model->vertices[face->v3].x, model->vertices[face->v3].y, model->vertices[face->v3].z); glTexCoord2f(model->texCoords[face->vt3].u, model->texCoords[face->vt3].v); glNormal3f(model->normals[face->vn3].x, model->normals[face->vn3].y, model->normals[face->vn3].z);
}

// Draw the model by applying each face
void drawModel(Model *model) {
    // apply face
    glPushMatrix();
        glBegin(GL_TRIANGLES);
            for (int i = 0; i < model->faces.size(); i++) {
                applyFace(model, &model->faces[i]);
            }
        glEnd();
    glPopMatrix();
}

// Load the model from an OBJ file
void loadModel(char* filename, Model* model) {
    // your code here   
}

void drawCubeHardCode() {
    glPushMatrix();
        glBegin(GL_TRIANGLES);
            // f1
            glVertex3f(-0.5,-0.5,0.5); glTexCoord2f(0,0); glNormal3f(0,0,1);
            glVertex3f(0.5,-0.5,0.5); glTexCoord2f(1,0); glNormal3f(0,0,1);
            glVertex3f(-0.5,0.5,0.5); glTexCoord2f(0,1); glNormal3f(0,0,1);
            // f2
            glVertex3f(-0.5,0.5,0.5); glTexCoord2f(0,1); glNormal3f(0,0,1);
            glVertex3f(0.5,-0.5,0.5); glTexCoord2f(1,0); glNormal3f(0,0,1);
            glVertex3f(0.5,0.5,0.5); glTexCoord2f(1,1); glNormal3f(0,0,1);
        glEnd();
    glPopMatrix();
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    drawCubeHardCode();

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
