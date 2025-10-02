/*
Joshua Bayt
301893593
Project 2

Controls:
UP_ARROW - Zoom In
DOWN_ARROW - Zoom Out
RIGHT_ARROW - Rotate Camera Right
LEFT_ARROW - Rotate Camera Left
SPACE - Cycle between objects (Selected object will appear WHITE and controls only affect it)
        (To select all objects, cycle through all of them)
*/
#include <functional> // Needed for std::function<> for lambda functions
#include <unordered_map> // Needed for std::unordered_map as lookup table
#include <string.h>
#include <vector> // Needed for dynamic array for storing entities to render
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
#include <tuple>
using namespace std;

// Shapes needed for hash table glutDrawMap. Enum used to give better lookup (better than 0,1,2, etc)
enum class Shape {
    Cube,
    Sphere,
    Teapot
};

// Color3f class for holding R,G,B info for glColor3f calls. Makes it easier than passing a tuple
// or 3 args to a function
struct Color3f {
    float r, g, b;
    Color3f() {
        setColor(0,0,0); // Monochromatic contructor
    }
    Color3f(float r, float g, float b) { // Chromatic constructor
        setColor(r,g,b);
    }
    Color3f(float m) { // Sets color (monochromatic)
        setColor(m);
    }
    void setColor(float r, float g, float b) { // Sets color (chromatic)
        this->r = r;
        this->g = g;
        this->b = b;
    }
    void setColor(float m) {
        r=m,g=m,b=m;
    }
};

// Entity Class, holds values need for transform, rotation, glutShapeDraw //
// makes it easy to add/remove objects as needed
struct entity {
    float x,y,z; // Transform
    float zm; // Zoom
    int r; // Rotation
    Color3f color; // Color
    Shape s; // GLUT Shape of Entity
    entity() { // Default constructor
        x = 0, y = 0, z = 0, zm = 0, r = 0;
        color.setColor(0);
        s = Shape::Teapot; // Default teapot
    }
    entity(float x, float y, float z, float zm, int r, Color3f color, Shape s) { // Override constructor
        this->x = x;
        this->y = y;
        this->z = z;
        this->zm = zm;
        this->r = r;
        setColor(color);
        this->s = s;
    }
    // Set color of entity (with Color3f)
    void setColor(Color3f color) {
        this->color = color;
    }
};

bool WireFrame= false;

// LIGHT GLOBAL VARIABLES //
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
// SELECTION VARIABLES //
int selectionIndex = -1; // -1 = ALL Selected

// OBJECTS //
entity sphere(-3.5,0.0,-1.0,0,0, Color3f(0,0,1),Shape::Sphere);
entity cube(4.0,0.0,-2.0,0,0,Color3f(0,1,0),Shape::Cube);
entity teapot(0.0,0.0,-3.0,0,0,Color3f(1,0,0),Shape::Teapot);


// GRAPHICS TABLE //
// Maps enum shape types to function, called in draw. Allows for more shapes to be easily added
// if needed and avoids giant if-else statements
const unordered_map<Shape,function<void()>> glutShapeTable = {
    // first is key, Shape entered in first parameter maps to the second (which is a function)
    // second is lambda function to be ran in place in draw function
    {Shape::Cube, [](){ glutSolidCube(2);}},
    {Shape::Sphere, [](){ glutSolidSphere(1.3, 15, 15);}},
    {Shape::Teapot, [](){ glutSolidTeapot(1.5);}}
};

// ENTITITY LIST //
// Dynamic array for holding entities
vector<entity> entities = {sphere,cube,teapot};


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

// Draws entity passed in first arg, second arg is condition on whether to draw white or normal
void draw(entity toDraw, bool selected) {
    // Check if entity is selected, if so draw it white
    if (selected) {
        glColor3f(1,1,1); // white
    } else {
        // if not selected, draw its natural color
        glColor3f(toDraw.color.r,toDraw.color.g,toDraw.color.b);
    }
    // Apply drawing to EACH entitity in the array
    glPushMatrix();
        // To do rotations around the viewer, we do translate to origin + added zoom, rotate, then translate back
        glTranslated(0,0,toDraw.zm); //We can't do global zoom because we manually select objects
        glRotated(toDraw.r,0,1,0); // only rotate around the Y-axis
        glTranslated(toDraw.x,toDraw.y,toDraw.z); // move object back to it's location (these values actually never change, but each object has different ones)
        auto it = glutShapeTable.find(toDraw.s); // retrieve function call for glutSolid from table
        it->second(); // call function after retrieval
    glPopMatrix();
}


static void display(void)
{
    // GLUT display function //
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,2,10,0.0,0.0,0.0,0.0,1.0,0.0); // Where camera is positioned

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME
    // your code here
    // Calls draw() on each entity in array
    for (int i = 0; i < entities.size(); i++) {
        if (i == selectionIndex) {
            draw(entities[i],true); // If entity is selected, color it white
        } else {
            draw(entities[i],false); // If entity is not selected, dont change the color
        }
    }
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
        // SPACE = Selection Slide (Goes through each element, -1 = all)
        case ' ':
            // if selection index is in bounds, increase it to selected next object
            if (selectionIndex < (int)entities.size()-1) {
                selectionIndex++;
            } else {
                // set to -1 (all) after we reach end of array
                selectionIndex = -1;
            }
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    // ZOOM KEYS //
    case GLUT_KEY_UP:
            if (selectionIndex >= 0) {
                entities[selectionIndex].zm+=0.5; // If entity selected, only modify that one
            } else {
                for (int i = 0; i < entities.size(); i++) {
                    entities[i].zm+=0.5; // If no entity selected, modify ALL of them
                }
            }
            break;
    case GLUT_KEY_DOWN:
            if (selectionIndex >= 0) {
                entities[selectionIndex].zm-=0.5; // If entity selected, only modify that one
            } else {
                for (int i = 0; i < entities.size(); i++) {
                    entities[i].zm-=0.5; // If no entity selected, modify ALL of them
                }
            }
            break;
    // ROTATION KEYS //
    case GLUT_KEY_RIGHT:
            if (selectionIndex >= 0) {
                entities[selectionIndex].r+=5; // If entity selected, only modify that one
            } else {
                for (int i = 0; i < entities.size(); i++) {
                    entities[i].r+=5; // If no entity selected, modify ALL of them
                }
            }
            break;
    case GLUT_KEY_LEFT:
            if (selectionIndex >= 0) {
                entities[selectionIndex].r-=5; // If entity selected, only modify that one
            } else {
                for (int i = 0; i < entities.size(); i++) {
                    entities[i].r-=5; // If no entity selected, modify ALL of them
                }
            }
            break;
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
    cout << "Hello World!";
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
