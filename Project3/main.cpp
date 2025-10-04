/* CONTROLS
    W -> 
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
        GLdouble theta = 0;
        GLdouble orbit_theta = 0;
        bool center_of_universe = false;
        GLuint tex;                                     // texture identification
        GLdouble radius;                                // radius of object
        char* tex_location;                                 // location of image for texture loader
        vector<Celestial*> children;                    // bodies that orbit ts
        GLdouble orbit_radius;                          // how far the object orbits around its "orbits" attribute
        GLUquadricObj* sphere = nullptr;             // required for sphere to allow texture
        GLdouble rotation_velocity;                 // how fast the object rotates individually
        GLdouble orbit_velocity;                    // how fast the object orbits around its parent (if it has one)
    Celestial(GLdouble radius, char* tex_location, GLdouble orbit_radius, GLdouble rotation_velocity, GLdouble orbit_velocity) {
        this->radius = radius;
        this->tex_location = tex_location;
        this->orbit_radius = orbit_radius;
        this->rotation_velocity = rotation_velocity;
        this->orbit_velocity = orbit_velocity;
    }
    // RPM of object = (orbit_velocity * 60 * 60) / 360
};

/* -- GLOBALS -- */

// Celestial sun(12,"images/sun.jpg",0,0.0039,0);
// Celestial mercury(0.38,"images/mercury.jpg",20,0.017,4.15);
// Celestial venus(0.95,"images/venus.jpg",30,-0.004,1.63);
// Celestial earth(1,"images/earth.jpg",45,1,1);
// Celestial moon(0.27,"images/moon.jpg",3,0.0366,0.075);
// Celestial mars(0.53,"images/mars.jpg",55,0.975,0.53);
// Celestial jupiter(6,"images/jupiter.jpg",70,2.418,0.084);
// Celestial saturn(3,"images/saturn.jpg",80,2.243,0.034);
// Celestial uranus(4,"images/uranus.jpg",90,-1.392,0.012);
// Celestial neptune(3.9,"images/neptune.jpg",100,1.49,0.0061);
// Celestial pluto(0.19,"images/pluto.jpg",120,-0.157,0.004);

GLdouble dist = 125;

Celestial sun(109,"images/sun.jpg",0,0.000,0);
Celestial mercury(0.38,"images/mercury.jpg",dist*1.00,0.017,4.15);
Celestial venus(0.95,"images/venus.jpg",dist*1.85,-0.004,1.63);
Celestial earth(1,"images/earth.jpg",dist*2.56,1,1);
Celestial moon(0.27,"images/moon.jpg",3,0.0366,0.075);
Celestial mars(0.53,"images/mars.jpg",dist*3.9,0.975,0.53);
Celestial jupiter(11,"images/jupiter.jpg",dist*13.3,2.418,0.084);
Celestial saturn(9.1,"images/saturn.jpg",dist*24.6,2.243,0.034);
Celestial uranus(4,"images/uranus.jpg",dist*49.2,-1.392,0.012);
Celestial neptune(3.9,"images/neptune.jpg",dist*77.2,1.49,0.0061);
Celestial pluto(0.19,"images/pluto.jpg",dist*101,-0.157,0.004);

vector<Celestial> space = {sun,mercury,venus,earth,moon,mars,jupiter,saturn,uranus,neptune,pluto};

bool WireFrame= false;
bool sim_running = false;

Vec3 camera_pos;
Vec3 camera_look;
Vec3 focusPos;

int selection = 0;
int theta = 45;

const double PI = 3.14159265358979323846;

double sim_speed = 1;

GLdouble r = 0;
GLdouble yaw = 4.55;
GLdouble pitch = 1.55;
GLdouble speed = 1;

int slices = 128;
int stacks = 128;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* --------------*/

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    GLdouble Ratio = (double)width/(double)height;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,Ratio,0.1f, 10000.0f);
 }

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

// Draws input object
void draw(const Celestial &toRender) {
    //cout << "\nDrawing: " << toRender.tex_location;
    //cout << "\nSize: " << toRender.children.size();
    glPushMatrix();
        if(toRender.center_of_universe) {
            glDisable(GL_LIGHTING);
            glColor4f(1,1,1,1);
            glBindTexture(GL_TEXTURE_2D,toRender.tex);
            glRotated(toRender.theta,0,1,0);
            gluSphere(toRender.sphere,toRender.radius,slices,stacks);
        } else {
            glEnable(GL_LIGHTING);
            glBindTexture(GL_TEXTURE_2D,toRender.tex);
            glRotated(toRender.orbit_theta,0,1,0);
            glTranslated(toRender.orbit_radius,0,0);
            glRotated(toRender.theta,0,1,0);
            gluSphere(toRender.sphere,toRender.radius,slices,stacks);
        }
        for (int i = 0; i < toRender.children.size(); i++) {
            draw(*toRender.children[i]);
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
    // camera_look.x = camera_pos.x + sin(pitch)*cos(yaw);
    // camera_look.y = camera_pos.y + cos(pitch);
    // camera_look.z = camera_pos.z + sin(pitch)*sin(yaw);
    Celestial &obj = space[selection];

    camera_look.x = obj.orbit_radius*cos(obj.orbit_theta*PI/180);
    camera_look.y = 0;
    camera_look.z = obj.orbit_radius*sin(obj.orbit_theta*PI/180);
    
    //camera_look = focusPos;

    camera_pos.x = r*cos(yaw)*sin(pitch) + camera_look.x;
    camera_pos.y = r*cos(pitch) - camera_look.y;
    camera_pos.z = r*sin(yaw)*sin(pitch) + camera_look.z;

    

    gluLookAt(
        camera_pos.x,camera_pos.y,camera_pos.z,         // eyeX, eyeY, eyeZ (camera pos)
        camera_look.x,camera_look.y,camera_look.z,    // centerX, centerY, centerZ (where camera is looking AT)
        0.0,1.0,0.0     // upX, upY, upZ (which way is up, keep same)
    );
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);        //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        //Toggle WIRE FRAME
    
    draw(space[0]);
    glutSwapBuffers();
    /*
    Why swap buffers? 
    Allows for one to load, one to render
    
    */
}

// prints debugging info
void debug() {
    system("cls");
    cout << "Camera Pos: (" << camera_pos.x << "," << camera_pos.y << "," << camera_pos.z << ")\n";
    cout << "Camera Look: (" << camera_look.x << "," << camera_look.y << "," << camera_look.z << ")\n";
    cout << "yaw: " << yaw << "\n";
    cout << "pitch: " << pitch << "\n";
    cout << "speed: " << speed << "\n";
    cout << "selected: " << space[selection].tex_location << "\n";
    cout << "sim_speed: " << sim_speed << "\n";
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
            //camera_strafe(1);
            r+=speed;
            break;
        case 's':
            //camera_strafe(-1);
            r-=speed;
            break;
        case '+':
            speed*=2;
            break;
        case '-':
            speed*=0.5;
            break;
        case ' ':
            sim_running = !sim_running;
            break;
        case '1':
            if (selection < space.size()-1) {
                selection++;
            } else {
                selection = 0;
            }
            break;
        case '2':
            sim_speed*=2;
            break;
        case '3':
            sim_speed*=0.5;
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
    if (sim_running) {
        auto t2 = chrono::high_resolution_clock::now();
        auto d = chrono::duration_cast<chrono::milliseconds>(t2-t1);
        // wait 16.66ms (60fps)
        if (d.count() > 16.66) {
            for (int i = 0; i < space.size(); i++) {
                Celestial &obj = space[i];
                obj.theta += obj.rotation_velocity*sim_speed;
                obj.orbit_theta += obj.orbit_velocity*sim_speed;
                if (obj.theta > 360) {
                    obj.theta = 0;
                }
                if (obj.orbit_theta > 360) {
                    obj.orbit_theta = 0;
                }
            }
            t1 = chrono::high_resolution_clock::now();
        }
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
    space[0].center_of_universe = true;
    space[0].children = {&space[1],&space[2],&space[3],&space[5],&space[6],&space[7],&space[8],&space[9],&space[10]};
    space[3].children = {&space[4]};
    camera_look.x = 0;
    camera_look.y = 0;
    camera_look.z = 0;
    //draw(space[0]);
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
