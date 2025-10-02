
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

#include <chrono> // better than time.h
#include <cmath>
#include <vector>

using namespace std;

/* -- Structs -- */
struct Vec3 {
    GLdouble x;
    GLdouble y;
    GLdouble z;
    Vec3() {
        x = y = z = 0;
    }
    Vec3(GLdouble x, GLdouble y, GLdouble z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

class Celestial {
    public:
        GLuint tex;                                     // texture identification
        GLdouble radius;                                // radius of object
        char* tex_location;                                 // location of image for texture loader
        Celestial* orbits = nullptr;                    // what the object orbits around (nullptr if none)
        GLdouble orbit_radius;                          // how far the object orbits around its "orbits" attribute
        GLUquadricObj* sphere = nullptr;             // required for sphere to allow texture
        GLdouble rotation_velocity;                 // how fast the object rotates individually
        GLdouble orbit_velocity;                    // how fast the object orbits around its parent (if it has one)
    Celestial(GLdouble radius, char* tex_location, Celestial* orbits, GLdouble orbit_radius, GLdouble rotation_velocity, GLdouble orbit_velocity) {
        this->radius = radius;
        this->tex_location = tex_location;
        this->orbits = orbits;
        this->orbit_radius = orbit_radius;
        this->rotation_velocity = rotation_velocity;
        this->orbit_velocity = orbit_velocity;
    }
};

/* -- GLOBALS -- */

Celestial sun(3,"images/sun.jpg",nullptr,0,1,0);
Celestial earth(1,"images/earth.jpg",&sun,16,1,1);
Celestial moon(0.3,"images/moon.jpg",&earth,2,1,1);
vector<Celestial> space = {sun,earth,moon};

bool WireFrame= false;

Vec3 camera_pos;
Vec3 camera_look;

int theta = 45;

GLdouble yaw = 4.55;
GLdouble pitch = 1.55;
GLdouble speed = 0.1;



/* --------------*/

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

void TextureLoader(char *fileName, GLuint tex){

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

// Draws input object
void draw(const Celestial &toRender) {
    glPushMatrix();
        if(toRender.orbits == nullptr) {
            // pivot = origin
            glBindTexture(GL_TEXTURE_2D,toRender.tex);
            glRotated((double)theta,0,1,0);
            gluSphere(toRender.sphere,toRender.radius,40,40);
        } else {
            // pivot = orbits
            glBindTexture(GL_TEXTURE_2D,toRender.tex);
            glTranslated(toRender.orbits->orbit_radius,0,0);
            glRotated((double)theta,0,1,0);
            glTranslated(toRender.orbit_radius,0,0);
            glRotated((double)theta,0,1,0);
            gluSphere(toRender.sphere,toRender.radius,40,40);
        }
    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    

    /*
    Uses Spherical Coordinates for camera look position:
        x = sin(theta) * cos*(phi)
        y = sin(theta) * sin(phi)
        z = cos(theta)

        However, this assumes in the graph Z goes up and down (for pitch) but in OpenGL since Z is in/out of the screen it is actualy Y that
        affect pitch now so we change the functions to match
    */
    camera_look.x = camera_pos.x + sin(pitch)*cos(yaw);
    camera_look.y = camera_pos.y + cos(pitch);
    camera_look.z = camera_pos.z + sin(pitch)*sin(yaw);
    
    gluLookAt(
        camera_pos.x,camera_pos.y,camera_pos.z,         // eyeX, eyeY, eyeZ (camera pos)
        camera_look.x,camera_look.y,camera_look.z,    // centerX, centerY, centerZ (where camera is looking AT)
        0.0,1.0,0.0     // upX, upY, upZ (which way is up, keep same)
    );

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);        //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        //Toggle WIRE FRAME
    
    for (int i = 0; i < space.size(); i++) {
        draw(space[i]);
    }
    // glPushMatrix();
    //     glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    //     glRotatef(-(float)theta,0,1,0);
    //     //glutSolidTeapot(0.75);
    //     gluSphere(sphere,2.0,40,40);
    //   // glutSolidSphere(1.5,25,25);
    // glPopMatrix();

    // glPushMatrix();
    //     glBindTexture(GL_TEXTURE_2D, tex1); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    //     glRotatef((float)theta,0,1,0);
    //     glTranslatef(4,0,0);
    //     glRotatef((float)theta,0,1,0);
    //     glutSolidTeapot(0.75);
    // glPopMatrix();

    glutSwapBuffers();
    /*
    Why swap buffers? 
    Allows for one to load, one to render
    
    */
}

// prints debugging info
void debug() {
    cout << "Camera Pos: (" << camera_pos.x << "," << camera_pos.y << "," << camera_pos.z << ")\n";
    cout << "Camera Look: (" << camera_look.x << "," << camera_look.y << "," << camera_look.z << ")\n";
    cout << "yaw: " << yaw << "\n";
    cout << "pitch: " << pitch << "\n";
    cout << "speed: " << speed << "\n";
}

void camera_strafe(int dir) {
    // dir = 1 or -1
    camera_pos.x+=speed*sin(pitch)*cos(yaw)*dir;
    camera_pos.y+=speed*cos(pitch)*dir;
    camera_pos.z+=speed*sin(pitch)*sin(yaw)*dir;
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'w':
            camera_strafe(1);
            break;
        case 's':
            camera_strafe(-1);
            break;
        case '+':
            speed+=0.1;
            break;
        case '-':
            speed-=0.1;
            break;
        
    }
    debug();
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        pitch-=0.05;
        break;
    case GLUT_KEY_DOWN:
        pitch+=0.05;
        break;
    case GLUT_KEY_RIGHT:
        yaw+=0.05;
        break;
    case GLUT_KEY_LEFT:
        yaw-=0.05;     
        break;
   }
   debug();
   glutPostRedisplay();
}

auto t1 = chrono::high_resolution_clock::now();
static void idle(void)
{
    auto t2 = chrono::high_resolution_clock::now();
    auto d = chrono::duration_cast<chrono::milliseconds>(t2-t1);
    // wait 16.66ms (60fps)
    if (d.count() > 16.66) {
        theta += 1;
        theta = theta%360;
        t1 = chrono::high_resolution_clock::now();
    }
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

  //  glEnable(GL_CULL_FACE);
  //  glCullFace(GL_BACK);
    
    glEnable(GL_TEXTURE_2D);
    

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    // Setup spheres for textyure loader
    for (int i = 0; i < space.size(); i++) {
        space[i].sphere = gluNewQuadric();
        gluQuadricDrawStyle(space[i].sphere, GLU_FILL);
        gluQuadricTexture(space[i].sphere, GL_TRUE);
        gluQuadricNormals(space[i].sphere, GLU_SMOOTH);
        glGenTextures(1, &space[i].tex);
        TextureLoader(space[i].tex_location,space[i].tex);
    }
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Solar Scene");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
