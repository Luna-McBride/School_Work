
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//general globals
int th=0;
int ph=0;
double dim=2;
int mode=1;

/*******lorenz globals*******/
//parameters
double s = 10;
double b = 2.6666;
double r = 28;

//Directly from Example 6 like others, but would not work without being here
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

/*
 * Changed functions between this and the Example 6/Lorenz Attractor file given. 
 * Some elements kept for functionality
 */

//A lorenz function using the given lorenz example with an added section to make points
void lorenz(double a, double c, double e){
    //iterator and accumulators
    int i;
    double dx,dy,dz;
    //coordinates
    double x=0.1;
    double y=0.1;
    double z=0.1;
    //change in time
    double dt=0.001;
    //applies math for each point and make it a point
    for(i=0;i<50000;i++){
        //gives the required point in the required equation
        dx=a*(y-x);
        dy=x*(e-z)-y;
        dz=x*y - c*z;
        x+=dt*dx;
        y+=dt*dy;
        z+=dt*dz;
        //makes a point for each one
        glBegin(GL_POINTS);
        glVertex3d(x,y,z);
        glEnd();
        //creates the art from modes 5-9
        if(mode == 5){
            glRotated(90,90,90,90);
        }
        else if(mode == 6){
            glRotated(45,90,135,180);
        }
        else if(mode == 7){
            glRotated(2,22,222,2222);
        }
        else if(mode == 8){
            glRotated(15,30,45,60);
        }
        else if(mode == 9){
            glRotated(66,13,246,173);
        }
    }
}

//basically display the objects
void display(){
    //clear whatever garbage it tries to pull up
    glClear(GL_COLOR_BUFFER_BIT);
    //clears any random transformations
    glLoadIdentity();
    //sets initial rotation conditions
    glRotated(ph,1,0,0);
    glRotated(th,0,1,0);
    glScaled(0.0333,0.0333,0.0333);

    //color and size of points
    glColor3f(1,0,0);
    glPointSize(1);
    //calls function to make lorenz
    lorenz(s,b,r);
    //alters color and makes another for modes 5-9
    if(mode == 5){
        glColor3f(1,0,1);
        lorenz(s,b,r);
    }
    else if(mode == 6){
        glColor3f(1,1,0);
        lorenz(s,b,r);
    }
    else if(mode == 7){
        glColor3f(0,1,1);
        lorenz(s,b,r);
    }
    else if(mode == 8){
        glColor3f(1,1,1);
        lorenz(s,b,r);
    }
    else if(mode == 9){
        glColor3f(0,1,0);
        lorenz(s,b,r);
    }

    /*****Sets the axies******/
    if(mode <=4){
        //makes the lines white
        glColor3f(1,1,1);
        //start making lines
        glBegin(GL_LINES);
        //makes x line
        glVertex3d(0,0,0);
        glVertex3d(30,0,0);
        //makes y line
        glVertex3d(0,0,0);
        glVertex3d(0,30,0);
        //makes z line
        glVertex3d(0,0,0);
        glVertex3d(0,0,30);
        //stop making lines
        glEnd();

        //puts the letter X on the x axis
        glRasterPos3d(30,0,0);
        Print("X");
        //puts the letter Y on the y axis
        glRasterPos3d(0,30,0);
        Print("Y");
        //puts the letter Z on the z axis
        glRasterPos3d(0,0,30);
        Print("Z");
    }

    //show parameters for testing and other usage	
    glWindowPos2i(5,5);
    Print("Angle: %d,%d", th, ph);
    if(mode == 1){
        Print("  Variable being changed: s=%.1f", s);
    }
    else if(mode == 2){
        Print("  Variable being changed: b=%.1f", b);
    }
    else if(mode == 3){
        Print("  Variable being changed: r=%.1f", r);
    }
    else if(mode == 4){
        Print("  Type: Normal Variables: s=%.1f b=%.1f r=%.1f", s,b,r);
    }
    else{
        Print("  Type: Art Variables: s=%.1f b=%.1f r=%.1f", s,b,r);
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
    else if('1'<=ch && ch<='9'){
        mode=ch-'0';
    }
    //increase the specified parameter
    else if(ch == '='){
        if(mode == 1){
            s+=0.1;
        }
        else if(mode == 2){
            b+=0.1;
        }
        else if(mode == 3){
            r+=0.1;
        }
        else{
            s+=0.1;
            b+=0.1;
            r+=0.1;
        }
    }
    //decrease the specified parameter
    else if(ch == '-'){
        if(mode == 1){
            s-=0.1;
        }
        else if(mode == 2){
            b-=0.1;
        }
        else if(mode == 3){
            r-=0.1;
        }
        else{
            s-=0.1;
            b-=0.1;
            r-=0.1;
        }
    }
    //reset parameters with the letter q
    else if(ch == 'q'){
        s=10;
        b=2.6666;
        r=28;
    }
    //redisplay the updated scene
    glutPostRedisplay();
}

/*
 * All from this point on follow Example 6 to a T, but with redone
 * comments so I can at least show I know what it is doing. 
 */

//handle special (non-alphabetic) keys being pressed
void special(int key,int x,int y){
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
    //modulo angles with 360 to keep within our usage of degrees (not radians)
    th %= 360;
    ph %= 360;
    //redisplay the scene
    glutPostRedisplay();
}

//change window shape
void reshape(int width, int height){
    //gets the ratio of the width to the window height
    double w2h = (height>0) ? (double)width/height : 1;
    //makes the entire area viewable
    glViewport(0,0,width,height);
    //allows the projection matrix to be changed
    glMatrixMode(GL_PROJECTION);
    //loads the identity matrix, resetting past transformations
    glLoadIdentity();
    //change orthogonal box for the window aspect ratio
    glOrtho(-dim*w2h, +dim*w2h, -dim,+dim, -dim,+dim);
    //allows the model matrix to be changed
    glMatrixMode(GL_MODELVIEW);
    //resets matrix to the identity
    glLoadIdentity();                      
    //Note: execution order:  ^
    //                        |
}

//start the program and GLUT
int main(int argc, char* argv[]){
    //start GLUT with pre-specified parameters
    glutInit(&argc,argv);
    //request the window to double buffer (have two running for
    //preceived speed) and true colors
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    //have starting window size be 500 pixels X 500 pixels
    glutInitWindowSize(500,500);
    //Start window with assignment name on top.
    glutCreateWindow("Assignment 1: Luna McBride");
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
