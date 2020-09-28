#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//mostly taken from Example 9
int pType=1;
int mode=9;
int th=0;
int ph=0;
int fov=55;
double asp=1;
double dim=6.0;
double dt=0.1;
double dx=1;
double dy=0;
double dz=0;
double Ex=-1;
double Ey=-1;
double Esx=-1;
double Esy=-1;
double Ez=1;
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

//Directly from HW1, and thus directly from Example 6.
#define LEN 8192 //longest string allowed
//allows text to be printed to the screen
void Print(const char* format, ...){
    //parameters defining
    char buf[LEN];
    char* ch=buf;
    va_list args; 

    //build the string from the parameters passed
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //display each character to display the word
    while(*ch){
        //maps each char to an 18pt helvetica font character on screen
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
    }
}

//  Skybox (Altered from Example 8)
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(0,1,1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,1,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(0,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(0,1,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

//taken from Example 8 (more to mess with camera and placement currently
static void Vertex(double t, double p){
    glVertex3d(Sin(t)*Cos(p), Sin(p), Cos(t)*Cos(p));
}

static void cone(double x, double y, double z, double r){
    int i,d=5;
    glColor3f(0.133,0.543,0.133);
    glPushMatrix();
    glRotated(90,1,0,0);
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glBegin(GL_TRIANGLE_STRIP);
    for (i=0;i<=360;i+=d){
        Vertex(i,0);
        glVertex3d(0,2,0);
    }
    glEnd();
    glPopMatrix();
}

//taken from Example 8
static void sphere(double x, double y, double z, double r){
    const int d=5;
    int t,p;

    //hold onto current transformation state
    glPushMatrix();
    //move and scale
    glTranslated(x,y,z);
    glScaled(r,r,r);

    //bands
    for(p=-90;p<90;p+=d){
        glBegin(GL_QUAD_STRIP);
        for(t=0;t<=360;t+=d){
            Vertex(t,p);
            Vertex(t,p+d);
        }
        glEnd();
    }

    //load save state
    glPopMatrix();   
}

static void leaf(double x, double y, double z,double rx,double ry,double rz, double r){
    const int d=5;
    int t,p;
    glColor3f(0.418,0.555,0.137);
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(90,rx,ry,rz);
    glScaled(2*r,r,r);
    for(p=-90;p<90;p+=d){
        glBegin(GL_QUAD_STRIP);
        for(t=0;t<=360;t+=d){
            Vertex(t,p);
            Vertex(t,p+d);
        }
        glEnd();
    }
    glPopMatrix();
}

static void circle(double x, double y, double z, double r){
    int i;
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glBegin(GL_POLYGON);
    for(i=0;i<=360;i+=5){
        Vertex(i,0);
    }
    glEnd();
    glPopMatrix();
}

static void can(double x, double y, double z,double r){
    const int d=5;
    int th,ph=0;
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(90,1,0,0);
    glColor3f(0.625,0.32,0.176);
    circle(0,0.45,0,0.92);
    glBegin(GL_QUAD_STRIP);
    for (ph=d-90;ph<=90;ph+=d){
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d){
         Vertex(th,ph/4);
         Vertex(th,(ph/4)+d);
      }
      glEnd();
    }
    glEnd();
    circle(0,-0.3,0,0.95);
    glPopMatrix();
    
}


static void tree(double x, double y, double z, double r){
    glPushMatrix();
    glScaled(r,r,r);
    glTranslated(x,y,z);
    can(3,3,0,1.5);
    can(3,3,1,1.4);
    can(3,3,2,1.3);
    can(3,3,3,1.2);
    can(3,3,3.5,1.1);
    can(3,3,4,1);
    can(3,3,4.3,0.9);
    can(3,3,4.6,0.8);
    can(3,3,4.9,0.7);
    can(3,3,5.2,0.6);
    can(3,3,5.5,0.5);
    can(3,3,5.7,0.4);
    can(3,3,5.9,0.3);
    can(3,3,6.1,0.2);
    can(3,3,6.2,0.1);
    leaf(1,3,6.6,0,0,0,1);
    leaf(5,3,6.6,0,0,0,1);
    leaf(3,1,6.6,0,0,1,1);
    leaf(3,5,6.6,0,0,1,1);
    glPopMatrix();
}

//Set the projection (Partially taken from Example 9)
static void Project(){
    //Alter the projection matrix specifically
    glMatrixMode(GL_PROJECTION);
    //Reset to identity
    glLoadIdentity();
    //set to orthogonal projection
    if(pType==1){
        glOrtho(-asp*dim,+asp*dim,-dim,+dim,-dim,+dim);
    }
    //for strictly perspective view
    else{
        gluPerspective(fov,asp,dim/4,dim*4);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void area(){
    if(pType!=3){
        glRotated(-90,1,0,0);
    }
    tree(3,11,0,1);
    tree(4,-10,0,2);
    tree(5,9,0,0.5);
    tree(-6,8,0,1.5);
    tree(7,7,0,0.25);
    tree(8,-6,0,0.5);
    tree(9,5,0,1);
    tree(-10, 4,0,0.1);
    tree(-11,-3,0,0.5);
    tree(-7,-7,0,0.25);
    tree(8,-6,0,0.5);
    tree(-9,-5,0,1);
    tree(-10, 4,0,0.1);
    tree(-4,5,0,0.5);
    tree(6,-5,0,1);
    tree(5,-4,0,0.1);
    tree(4,-5,0,0.5);
    tree(13,21,0,1);
    tree(14,-20,0,2);
    tree(15,19,0,0.5);
    tree(-16,18,0,1.5);
    tree(17,17,0,0.25);
    tree(18,-16,0,0.5);
    tree(19,15,0,1);
    tree(-20, 14,0,0.1);
    tree(-21,-13,0,0.5);
    tree(-17,-17,0,0.25);
    tree(18,-16,0,0.5);
    tree(-19,-15,0,1);
    tree(-20, 14,0,0.1);
    tree(-14,15,0,0.5);
    tree(16,-15,0,1);
    tree(15,-14,0,0.1);
    tree(14,-5,0,0.5);
    tree(20,0,0,12);
    glColor3f(0.418,0.555,0.137);
    sphere(2,4,0,0.5);
    sphere(-2,-4,0,0.4);
    sphere(4,-4,0,0.3);
    sphere(8,-4,0,0.2);
    sphere(1,-4,0,0.2);
    sphere(12,14,0,0.5);
    sphere(-12,-14,0,0.4);
    sphere(14,-14,0,0.3);
    sphere(18,-14,0,0.2);
    sphere(11,-14,0,0.2);
    cone(6,0,6,1);
    cone(6,0,-6,1);
    cone(7,0,-3,0.5);
    cone(2,0,-9,0.4);
    cone(-12,0,9,0.3);
    cone(15,0,11,2);
    cone(-4,0,15,0.25);
    cone(-12,0,11,0.1);
    glColor3f(1,1,1);
    sphere(1,1,1,0.4);
    cube(Ex,Ey,Ez+8.9,10,10,10,90);
    glColor3f(0.625,0.32,0.176);
    glPushMatrix();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glRotated(91,1,0,0);
    glPolygonOffset(2,2);
    circle(0,0,0,1.1);
    glColor3f(1,1,1);
    glPolygonOffset(1,1);
    circle(0,0,0,1);
    glPolygonOffset(0,0);
    glColor3f(0,0,0);
    circle(0,0,0,0.1);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPopMatrix();
}

//basically display the objects (taken from HW1, modified with Example 9)
void display(){
    const double len=1.5;
    //clear whatever garbage it tries to pull up
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    //ZBUFFER
    glEnable(GL_DEPTH_TEST);
    //clears any random transformations
    glLoadIdentity();
    //sets initial rotation conditions
    if(pType==1){
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);   
    }
    else if(pType==2){
        double E1=-2*dim*Sin(th)*Cos(ph);
        double E2=+2*dim*Sin(ph);
        double E3=+2*dim*Cos(th)*Cos(ph);
        gluLookAt(E1,E2,E3,0,0,0,0,Cos(ph),0);
    }
    else if(pType==3){
        gluLookAt(Ex,Ey,Ez,Ex+dx,Ey+dy,Ez+dz,0,0,1);
    }
    
    area();
    


    if(mode==9){
        //makes the lines white
        glColor3f(1,1,1);
        //start making lines
        glBegin(GL_LINES);
        //makes x line
        glVertex3d(0,0,0);
        glVertex3d(len,0,0);
        //makes y line
        glVertex3d(0,0,0);
        glVertex3d(0,len,0);
        //makes z line
        glVertex3d(0,0,0);
        glVertex3d(0,0,len);
        //stop making lines
        glEnd();

        //puts the letter X on the x axis
        glRasterPos3d(len,0,0);
        Print("X");
        //puts the letter Y on the y axis
        glRasterPos3d(0,len,0);
        Print("Y");
        //puts the letter Z on the z axis
        glRasterPos3d(0,0,len);
        Print("Z");
    }

    if(mode<11){
        //show parameters for testing and other usage
        glColor3f(1,0,0);	
        glWindowPos2i(5,5);
        Print("Angle: %d,%d   Dim: %.1f   Fov: %d", th, ph, dim,fov);
        if(pType == 1){
            Print("  Projection: Orthogonal");
        }
        else if(pType == 2){
            Print("  Projection: Perspective");
        }
        else{
            Print("  Projection: First Person");
        }
    }


    //flush and swap buffers
    glFlush();
    glutSwapBuffers();
}

//registers regular key presses
void keyboard(unsigned char ch, int x, int y){
    //easy exit if escape is pressed
    if(ch == 27){
        exit(0);
    }
    //reset view
    else if(ch == '0'){
        th=0;
        ph=0;
    }
    //change the mode for manipulation
    else if('1'<=ch && ch<='3'){
        if(pType==3){
            Esx=Ex;
            Esy=Ey;
            Ex=-1;
            Ey=-1;
        }
        pType=ch-'0';
        if(pType==3){
            Ex=Esx;
            Ey=Esy;
            Esx=-1;
            Esy=-1;
            ph=th=0;
        }
    }
    else if(ch=='9'){
        if(mode==9){
            mode=0;
        }
        else{
            mode=9;
        }
    }
    else if(pType==3){
        if((ch=='w') || (ch=='W')){
            Ex+=dt*dx;
            Ey+=dt*dy; 
        }
        else if((ch=='s') || (ch=='S')){
            Ex-=dt*dx;
            Ey-=dt*dy;
        }
        else if((ch=='q') || (ch=='Q')){
            Ex=-1;
            Esx=-1;
            Ey=-1;
            Esy=-1;
        }
    }
    else{
        //increase the specified parameter
        if(ch == '='){
            dim+=0.1;
        }
        //decrease the specified parameter
        else if(ch == '-'){
            dim-=0.1;
        }
        else if(ch == 'p'){
            fov+=1;
        }
        else if(ch == 'o'){
            fov-=1;
        }
    } 
    Project();
    //redisplay the updated scene
    glutPostRedisplay();
}

/*
 * All from this point on follow Example 6 to a T, but with redone
 * comments so I can at least show I know what it is doing. 
 */

//handle special (non-alphabetic) keys being pressed
void special(int key,int x,int y){
    
    if(pType==3){
        //turns everything to the right if the right arrow is pressed
        if (key == GLUT_KEY_RIGHT){
            th-=5;
            dx=Cos(th);
            dy=Sin(th);
        }
        //turns everything to the left if the left arrow is pressed
        else if(key == GLUT_KEY_LEFT){
            th+=5;
            dx=Cos(th);
            dy=Sin(th);
        }
        //moves the object upward (y axis away) when the up arrow is pressed
        else if(key == GLUT_KEY_UP){
            if(ph<40){
                ph += 5;
                dz=Sin(ph);
            }
        }
        //moves the object downward (y axis toward) when the down arrow is pressed
        else if(key == GLUT_KEY_DOWN){
            if(ph>-5){
                ph -= 5;
                dz=Sin(ph);
            }
        }
    }
    else{
        //turns everything to the right if the right arrow is pressed
        if (key == GLUT_KEY_RIGHT){
            th += 5;
        }
        //turns everything to the left if the left arrow is pressed
        else if(key == GLUT_KEY_LEFT){
            th -= 5;
        }
        //moves the object upward (y axis away) when the up arrow is pressed
        else if(key == GLUT_KEY_UP){
            ph += 5;
        }
        //moves the object downward (y axis toward) when the down arrow is pressed
        else if(key == GLUT_KEY_DOWN){
            ph -= 5;
        }
    }
    
    //modulo angles with 360 to keep within our usage of degrees (not radians)
    th %= 360;
    ph %= 360;
    Project();
    //redisplay the scene
    glutPostRedisplay();
}

//change window shape (Taken from HW1, Modified with Example 9)
void reshape(int width, int height){
    //gets the ratio of the width to the window height
    asp = (height>0) ? (double)width/height : 1;
    //makes the entire area viewable
    glViewport(0,0,width,height);
    //Special function to set the projection
    Project();                     
    //Note: execution order:  ^
    //                        |
}

//start the program and GLUT (Taken from HW1 and modified)
int main(int argc, char* argv[]){
    //start GLUT with pre-specified parameters
    glutInit(&argc,argv);
    //request the window to double buffer (have two running for
    //preceived speed) and true colors
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    //have starting window size be 500 pixels X 500 pixels
    glutInitWindowSize(500,500);
    //Start window with assignment name on top.
    glutCreateWindow("Assignment 2: Luna McBride");
    //call display once loaded
    glutDisplayFunc(display);
    //call reshape when window is changed
    glutReshapeFunc(reshape);
    //calls the special function for special keys
    glutSpecialFunc(special);
    //calls the non-special function for non-special keys
    glutKeyboardFunc(keyboard);
    //give GLUT the user interaction control
    glutMainLoop();
    //end
    return 0;
}
