/**
* CSCI 172 - Project 5 - OBJ Model Loader and Skybox
* Joshua Bayt
 */
#include <string.h>
#include <string>

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

#include <fstream>
#include <sstream>

using namespace std;

/* ENUMS */

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
        enum FaceType {
           VERTEX,
           VERTEX_TEXCOORD,
           VERTEX_NORMAL,
           VERTEX_TEXCOORD_NORMAL            
        };
        FaceType type;
        int v1, v2, v3;       // vertex indices
        int vt1, vt2, vt3;   // texture coordinate indices
        int vn1, vn2, vn3;   // vertex normal indices
};

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
     cout << "Texture loaded: " << fileName << "\n";
}


class Model {
    public:
        bool loaded = false;
        string modelFileName = "";
        char* imageFileName = "";
        GLuint tex;
        Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
        vector<Vertex3f> vertices;
        vector<VertexTex2f> texCoords;
        vector<VertexNormal3f> normals;
        vector<Face3D> faces;
        Model(string fileName, char* imageFileName) {
            this->modelFileName = fileName;
            this->imageFileName = imageFileName;
        }
        void setupTextures() {
            glGenTextures(1, &tex);
            TextureLoader(imageFileName, tex);
        }
        void debugPrint() {
            system("cls");
            cout << "Model Name: " << modelFileName << "\n";
            cout << "Model Image: " << imageFileName << "\n";
            cout << vertices.size() << " vertices, " << texCoords.size() << " texture coords, " << normals.size() << " normals, " << faces.size() << " faces.\n";
            cout << "Model Size: " << sizeof(*this) + (sizeof(Vertex3f) * vertices.size()) + (sizeof(VertexTex2f) * texCoords.size()) + (sizeof(VertexNormal3f) * normals.size()) + (sizeof(Face3D) * faces.size()) << " bytes\n";
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
                switch (faces[i].type) {
                    case Face3D::VERTEX:
                        printf("f %i %i %i\n", 
                            faces[i].v1, faces[i].v2, faces[i].v3
                        );
                        break;
                    case Face3D::VERTEX_NORMAL:
                        printf("f %i//%i %i//%i %i//%i\n", 
                            faces[i].v1, faces[i].vn1,
                            faces[i].v2, faces[i].vn2,
                            faces[i].v3, faces[i].vn3
                        );
                        break;
                    case Face3D::VERTEX_TEXCOORD:
                        printf("f %i/%i %i/%i %i/%i\n", 
                            faces[i].v1, faces[i].vt1,
                            faces[i].v2, faces[i].vt2, 
                            faces[i].v3, faces[i].vt3
                        );
                        break;
                    case Face3D::VERTEX_TEXCOORD_NORMAL:
                        printf("f %i/%i/%i %i/%i/%i %i/%i/%i\n", 
                            faces[i].v1, faces[i].vt1, faces[i].vn1,
                            faces[i].v2, faces[i].vt2, faces[i].vn2,
                            faces[i].v3, faces[i].vt3, faces[i].vn3
                        );
                        break;
                }                
            }
        }
};

// Skybox Class
class SkyBox {
    public:
        GLuint tex[6]; // Texture IDs
        Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f position = Vector3f(0.0f, 0.0f, 5.0f);
        Vector3f scale = Vector3f(30.0f, 5.0f, 30.0f);
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


Model* cube = new Model("models/cube.obj", "images/chud.jpg");
Model* trunk = new Model("models/trunk.obj", "images/chud.jpg");
Model* bunny = new Model("models/bunny.obj", "images/chud.jpg");


SkyBox *skybox = new SkyBox();

vector<Model*> models = {cube, trunk, bunny};

char* skyboxFiles[6] = {
    "images/skybox/front.jpg",
    "images/skybox/back.jpg",
    "images/skybox/top.jpg",
    "images/skybox/bottom.jpg",
    "images/skybox/right.jpg",
    "images/skybox/left.jpg"
};

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    GLdouble Ratio = (double)width/(double)height;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,Ratio,0.1f, 19999.0f);
 }

// Apply a single face of the model
void applyFace(Model *model, Face3D* face) {
    if (face->type == Face3D::VERTEX) {
        // VERTEX case
        glVertex3f(model->vertices[face->v1-1].x, model->vertices[face->v1-1].y, model->vertices[face->v1-1].z); 
        glVertex3f(model->vertices[face->v2-1].x, model->vertices[face->v2-1].y, model->vertices[face->v2-1].z); 
        glVertex3f(model->vertices[face->v3-1].x, model->vertices[face->v3-1].y, model->vertices[face->v3-1].z); 
    } else if (face->type == Face3D::VERTEX_NORMAL) {
        // VERTEX_NORMAL case
        glNormal3f(model->normals[face->vn1-1].x, model->normals[face->vn1-1].y, model->normals[face->vn1-1].z); glVertex3f(model->vertices[face->v1-1].x, model->vertices[face->v1-1].y, model->vertices[face->v1-1].z); 
        glNormal3f(model->normals[face->vn2-1].x, model->normals[face->vn2-1].y, model->normals[face->vn2-1].z); glVertex3f(model->vertices[face->v2-1].x, model->vertices[face->v2-1].y, model->vertices[face->v2-1].z); 
        glNormal3f(model->normals[face->vn3-1].x, model->normals[face->vn3-1].y, model->normals[face->vn3-1].z); glVertex3f(model->vertices[face->v3-1].x, model->vertices[face->v3-1].y, model->vertices[face->v3-1].z);
    } else if (face->type == Face3D::VERTEX_TEXCOORD) {
        // VERTEX_TEXCOORD case
        glTexCoord2f(model->texCoords[face->vt1-1].u, model->texCoords[face->vt1-1].v); glVertex3f(model->vertices[face->v1-1].x, model->vertices[face->v1-1].y, model->vertices[face->v1-1].z);
        glTexCoord2f(model->texCoords[face->vt2-1].u, model->texCoords[face->vt2-1].v); glVertex3f(model->vertices[face->v2-1].x, model->vertices[face->v2-1].y, model->vertices[face->v2-1].z);
        glTexCoord2f(model->texCoords[face->vt3-1].u, model->texCoords[face->vt3-1].v); glVertex3f(model->vertices[face->v3-1].x, model->vertices[face->v3-1].y, model->vertices[face->v3-1].z);
    }
    else {
        // VERTEX_TEXCOORD_NORMAL case
        glTexCoord2f(model->texCoords[face->vt1-1].u, model->texCoords[face->vt1-1].v); glNormal3f(model->normals[face->vn1-1].x, model->normals[face->vn1-1].y, model->normals[face->vn1-1].z); glVertex3f(model->vertices[face->v1-1].x, model->vertices[face->v1-1].y, model->vertices[face->v1-1].z); 
        glTexCoord2f(model->texCoords[face->vt2-1].u, model->texCoords[face->vt2-1].v); glNormal3f(model->normals[face->vn2-1].x, model->normals[face->vn2-1].y, model->normals[face->vn2-1].z); glVertex3f(model->vertices[face->v2-1].x, model->vertices[face->v2-1].y, model->vertices[face->v2-1].z); 
        glTexCoord2f(model->texCoords[face->vt3-1].u, model->texCoords[face->vt3-1].v); glNormal3f(model->normals[face->vn3-1].x, model->normals[face->vn3-1].y, model->normals[face->vn3-1].z); glVertex3f(model->vertices[face->v3-1].x, model->vertices[face->v3-1].y, model->vertices[face->v3-1].z);
    }
}
void drawModel(Model *model) {
    if (model->loaded) {
        glPushMatrix();
            glRotatef(model->rotation.x, 1.0f, 0.0f, 0.0f); glRotatef(model->rotation.y, 0.0f, 1.0f, 0.0f); glRotatef(model->rotation.z, 0.0f, 0.0f, 1.0f);
            glTranslatef(model->position.x, model->position.y, model->position.z);
            glBindTexture(GL_TEXTURE_2D, model->tex);
            glBegin(GL_TRIANGLES);
                for (int i = 0; i < model->faces.size(); i++) {
                    applyFace(model, &model->faces[i]);
                }
            glEnd();
        glPopMatrix();
    }
}

void setFace(string f1, string f2, string f3, Face3D* face) {   
    size_t p1 = f1.find('/');
    size_t p2 = f1.find('/', p1 + 1);
    if (p1 == string::npos) {
        // VERTEX case
        face->type = Face3D::VERTEX;
        // This case is easy, we just call stoi on each entry
        face->v1 = stoi(f1);
        face->v2 = stoi(f2);
        face->v3 = stoi(f3);
    } else if (p2 == string::npos) {
        // VERTEX_TEXCOORD case (f 1/1 2/2 3/3)
        face->type = Face3D::VERTEX_TEXCOORD;
        size_t slash = f1.find('/');
        string left = f1.substr(0, slash);
        string right = f1.substr(slash+1);
        face->v1 = stoi(left);
        face->vt1 = stoi(right);

        slash = f2.find('/');
        left = f2.substr(0, slash);
        right = f2.substr(slash+1);
        face->v2 = stoi(left);
        face->vt2 = stoi(right);

        slash = f3.find('/');
        left = f3.substr(0, slash);
        right = f3.substr(slash+1);
        face->v3 = stoi(left);
        face->vt3 = stoi(right);
    } else if (p1 + 1 == p2) {
        // VERTEX_NORMAL case (f 1//1 2//2 3//3)al
        face->type = Face3D::VERTEX_NORMAL; 
        size_t slash = f1.find('/');
        string left = f1.substr(0, slash);
        string right = f1.substr(slash+2);
        face->v1 = stoi(left);
        face->vn1 = stoi(right);

        slash = f2.find('/');
        left = f2.substr(0, slash);
        right = f2.substr(slash+2);
        face->v2 = stoi(left);
        face->vn2 = stoi(right);

        slash = f3.find('/');
        left = f3.substr(0, slash);
        right = f3.substr(slash+2);
        face->v3 = stoi(left);
        face->vn3 = stoi(right);
    } else {
        // VERTEX_TEXCOORD_NORMAL case (f 1/1/1 2/2/2 3/3/3)
        face->type = Face3D::VERTEX_TEXCOORD_NORMAL;
        size_t slash1 = f1.find('/');
        string left = f1.substr(0, slash1);
        size_t slash2 = f1.find('/', slash1 + 1);
        string middle = f1.substr(slash1+1, slash2 - slash1 - 1);
        string right = f1.substr(slash2+1);
        face->v1 = stoi(left);
        face->vt1 = stoi(middle);
        face->vn1 = stoi(right);

        slash1 = f2.find('/');
        left = f2.substr(0, slash1);
        slash2 = f2.find('/', slash1 + 1);
        middle = f2.substr(slash1+1, slash2 - slash1 - 1);
        right = f2.substr(slash2+1);
        face->v2 = stoi(left);
        face->vt2 = stoi(middle);
        face->vn2 = stoi(right);

        slash1 = f3.find('/');
        left = f3.substr(0, slash1);
        slash2 = f3.find('/', slash1 + 1);
        middle = f3.substr(slash1+1, slash2 - slash1 - 1);
        right = f3.substr(slash2+1);
        face->v3 = stoi(left);
        face->vt3 = stoi(middle);
        face->vn3 = stoi(right);
    }      
}

// Load the model from an OBJ file
bool loadModel(Model* model) {
    try {
        cout << "Loading model from " << model->modelFileName << "...\n";
        ifstream file(model->modelFileName);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << model->modelFileName << "\n";
            return false;
        }
        string line;
        int lineNumber = 0;
        while (getline(file, line)) {
            lineNumber++;
            if (!(line.empty() || line[0] == '#')) {    // Skip empty lines and comments ('#')           
                istringstream ss(line);                 // Create a stringstream for each line (treats each line like a mini file!)
                string tag;                             // in a SS, using >> extracts whitespace separated words, since we have (ex/ v -0.5 -0.5 0.5), this makes (v) (-0.5) (-0.5) (0.5) each a separate word
                ss >> tag;                              // ss >> tag extracts the first word into tag, thus it will either be (v, vt, vn, f) - anything else is invalid. (We don't use line[0] because the tag can contain multiple characters)           
                // VERTEX //
                if (tag == "v") {
                    float x, y, z;
                    ss >> x >> y >> z;                  // StringStream doesn't require stoi or stof, etc it auto converts it using extraction.
                    model->vertices.push_back(Vertex3f(x, y, z));
                // TEXTURE COORDINATE //
                } else if (tag == "vt") {
                    float u, v;
                    ss >> u >> v;
                    model->texCoords.push_back(VertexTex2f(u, v));
                // VERTEX NORMAL //
                } else if (tag == "vn") {
                    float nx, ny, nz;
                    ss >> nx >> ny >> nz;
                    model->normals.push_back(VertexNormal3f(nx, ny, nz));
                // FACE //
                } else if (tag == "f") {
                    /*
                    The faces are a bit trickier, they do not have whitespace separated values, but instead us '/', but use spaces to separate each entry (9 values total!)
                    In addition: there are different combinations,
                        f 1 2 3                     (vertex)
                        f 3/1 4/2 5/3               (vertex, texture coordinate)
                        f 6/4/1 3/5/3 7/6/5         (vertex, texture coordinate, vertex normal)
                        f 7//1 8//2 9//3            (vertex, vertex normal)
                        - https://en.wikipedia.org/wiki/Wavefront_.obj_file -
                    Finally, a Face can either be a Quad or a Triangle, we only support Triangles here.
                    */                   
                    Face3D* face = new Face3D();
                    string t1, t2, t3;
                    ss >> t1 >> t2 >> t3;
                    setFace(t1, t2, t3, face);
                    model->faces.push_back(*face);
                } else {
                    cout << "Invalid tag in OBJ file at line " << lineNumber << "\n";
                }      
            } 
        }
        model->loaded = true;
        cout << "Model loaded: " << model->vertices.size() << " vertices, " << model->texCoords.size() << " texture coords, " << model->normals.size() << " normals, " << model->faces.size() << " faces.\n";
        model->setupTextures();
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
    glDepthMask(GL_FALSE); // Needed for skybox
    skybox->drawSkybox(false);   
    glDepthMask(GL_TRUE);

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
        // Exits program
        case 'q':
            exit(0);
            break;
        // Debugs currently selected model
        case '~':
            models[modelIndex]->debugPrint();
            break;
        // Zooms camera in
        case 'w':
            cameraPos.z -= 1.0f;
            break;
        // Zooms camera out
        case 's':
            cameraPos.z += 1.0f;
            break;
        // Rotates skybox left
        case 'a':
            skybox->rotation.y -= 2.0f;
            break;
        // Rotates skybox right
        case 'd':
            skybox->rotation.y += 2.0f;
            break;
        case '/':
            WireFrame = !WireFrame;
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

    glEnable(GL_TEXTURE_2D);

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

    skybox->setupTextures(skyboxFiles);
    for (int i = 0; i < models.size(); i++) {
        loadModel(models[i]);
    }
}

void MouseFunction(int button, int state, int x, int y) {
    switch(button) {
        // Switches currently selected model
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN) {
                // Left button pressed
                if (modelIndex >= models.size()-1) {
                modelIndex = 0;
                } else {
                    modelIndex++;
                }
                cout << "Switched to model index[" << modelIndex << "] " << models[modelIndex]->modelFileName << " Loaded: " << (models[modelIndex]->loaded ? "true" : "false") <<"\n";
            }
            break;
    }
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
    glutMouseFunc(MouseFunction);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
