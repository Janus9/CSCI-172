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

/* CLASSES */

class Vector3f {
    public:
        GLfloat x,y,z;
        Vector3f() {
            x = 0.0f; y = 0.0f; z = 0.0f;
        }
        Vector3f(GLfloat x, GLfloat y, GLfloat z) {
            this->x = x; this->y = y; this->z = z;
        }
};

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

class Face3D {
    public:
        int v1, v2, v3;       // vertex indices
        int vt1, vt2, vt3;   // texture coordinate indices
        int vn1, vn2, vn3;   // vertex normal indices
};

class Model {
    public:
        bool loaded = false;
        string fileName = "";
        Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
        vector<Vertex3f> vertices;
        vector<VertexTex2f> texCoords;
        vector<VertexNormal3f> normals;
        vector<Face3D> faces;
        Model(string fileName) {
            this->fileName = fileName;
        }
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

int modelIndex = 0;

Vector3f cameraPos = Vector3f(0.0f, 0.0f, 5.0f);

Model* ateneam2 = new Model("models/ateneam-2.obj");
Model* cow = new Model("models/cow.obj");
Model* cube = new Model("models/cube.obj");
Model* ele = new Model("models/ele.obj");
Model* trunk = new Model("models/trunk.obj");
Model* ven2 = new Model("models/ven-2.obj");
Model* bunny = new Model("models/bunny.obj");

vector<Model*> models = {ateneam2, cow, cube, ele, trunk, ven2, bunny};


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
    glVertex3f(model->vertices[face->v1-1].x, model->vertices[face->v1-1].y, model->vertices[face->v1-1].z); glTexCoord2f(model->texCoords[face->vt1-1].u, model->texCoords[face->vt1-1].v); glNormal3f(model->normals[face->vn1-1].x, model->normals[face->vn1-1].y, model->normals[face->vn1-1].z);
    glVertex3f(model->vertices[face->v2-1].x, model->vertices[face->v2-1].y, model->vertices[face->v2-1].z); glTexCoord2f(model->texCoords[face->vt2-1].u, model->texCoords[face->vt2-1].v); glNormal3f(model->normals[face->vn2-1].x, model->normals[face->vn2-1].y, model->normals[face->vn2-1].z);
    glVertex3f(model->vertices[face->v3-1].x, model->vertices[face->v3-1].y, model->vertices[face->v3-1].z); glTexCoord2f(model->texCoords[face->vt3-1].u, model->texCoords[face->vt3-1].v); glNormal3f(model->normals[face->vn3-1].x, model->normals[face->vn3-1].y, model->normals[face->vn3-1].z);
}

// Draw the model by applying each face
void drawModel(Model *model) {
    if (model->loaded) {
        glPushMatrix();
            glRotatef(model->rotation.x, 1.0f, 0.0f, 0.0f); glRotatef(model->rotation.y, 0.0f, 1.0f, 0.0f); glRotatef(model->rotation.z, 0.0f, 0.0f, 1.0f);
            glTranslatef(model->position.x, model->position.y, model->position.z);
            glBegin(GL_TRIANGLES);
                for (int i = 0; i < model->faces.size(); i++) {
                    applyFace(model, &model->faces[i]);
                }
            glEnd();
        glPopMatrix();
    }
}

// Load the model from an OBJ file
bool loadModel(Model* model) {
    try {
        cout << "Loading model from " << model->fileName << "...\n";
        ifstream file(model->fileName);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << model->fileName << "\n";
            return false;
        }
        string line;
        while (getline(file, line)) {
            if (!(line.empty() || line[0] == '#')) {             
                istringstream ss(line);
                string tag;
                ss >> tag;
                if (tag == "v") {
                    float x, y, z;
                    ss >> x >> y >> z;
                    //cout << "v: " << x << " " << y << " " << z << "\n";
                    model->vertices.push_back(Vertex3f(x, y, z));
                } else if (tag == "vt") {
                    float u, v;
                    ss >> u >> v;
                    //cout << "vt: " << u << " " << v << "\n";
                    model->texCoords.push_back(VertexTex2f(u, v));
                } else if (tag == "vn") {
                    float nx, ny, nz;
                    ss >> nx >> ny >> nz;
                    //cout << "vn: " << nx << " " << ny << " " << nz << "\n";
                    model->normals.push_back(VertexNormal3f(nx, ny, nz));
                } else if (tag == "f") {
                    Face3D* face = new Face3D();
                    string t1, t2, t3;
                    ss >> t1 >> t2 >> t3;
                    size_t s1, s2;
                    // parse t1
                    s1 = t1.find('/');
                    s2 = t1.find('/', s1 + 1);
                    face->v1 = stoi(t1.substr(0, s1));
                    face->vt1 = stoi(t1.substr(s1 + 1, s2 - s1 - 1));
                    face->vn1 = stoi(t1.substr(s2 + 1));
                    // parse t2
                    s1 = t2.find('/');
                    s2 = t2.find('/', s1 + 1);
                    face->v2 = stoi(t2.substr(0, s1));
                    face->vt2 = stoi(t2.substr(s1 + 1, s2 - s1 - 1));
                    face->vn2 = stoi(t2.substr(s2 + 1));
                    // parse t3
                    s1 = t3.find('/');
                    s2 = t3.find('/', s1 + 1);
                    face->v3 = stoi(t3.substr(0, s1));
                    face->vt3 = stoi(t3.substr(s1 + 1, s2 - s1 - 1));
                    face->vn3 = stoi(t3.substr(s2 + 1));
                    model->faces.push_back(*face);
                }
            } 
        }
        model->loaded = true;
        cout << "Model loaded: " << model->vertices.size() << " vertices, " << model->texCoords.size() << " texture coords, " << model->normals.size() << " normals, " << model->faces.size() << " faces.\n";
        return true;
    } catch (const exception& e) {
        cerr << "Error: Cannot load model, reason: " << e.what() << "\n";
        return false;
    }
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

    gluLookAt(
        cameraPos.x, cameraPos.y, cameraPos.z,  // Camera position
        0.0, 0.0, 0.0,                          // Look-at point
        0.0, 1.0, 0.0);                         // Up vector


    //drawCubeHardCode();
    drawModel(models[modelIndex]);

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
        // Loads currently selected model
        case 'l':
            if (models[modelIndex]->loaded == false) {
                loadModel(models[modelIndex]);
            } else {
                cout << "Model already loaded.\n";
            }
            break;
        // Switches currently selected model
        case 'p':
            if (modelIndex >= models.size()-1) {
                modelIndex = 0;
            } else {
                modelIndex++;
            }
            cout << "Switched to model index[" << modelIndex << "] " << models[modelIndex]->fileName << " Loaded: " << (models[modelIndex]->loaded ? "true" : "false") <<"\n";
            break;
        // Debugs currently selected model
        case 'o':
            models[modelIndex]->debugPrint();
            break;
        // Zooms camera in
        case 'w':
            cameraPos.z += 1.0f;
            break;
        // Zooms camera out
        case 's':
            cameraPos.z -= 1.0f;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        models[modelIndex]->rotation.x -= 2.0f;
        break;
    case GLUT_KEY_DOWN:
        models[modelIndex]->rotation.x += 2.0f;
        break;
    case GLUT_KEY_LEFT:
        models[modelIndex]->rotation.y -= 2.0f;
        break;
    case GLUT_KEY_RIGHT:
        models[modelIndex]->rotation.y += 2.0f;
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
