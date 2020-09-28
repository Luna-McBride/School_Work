//! items included from the graphics class
#include "CSCIx229.h"

//! Classes created for OOP and Classes
#include "flyweight.hpp"
#include "shapes.hpp"
#include "Tree.hpp"
#include "firstPerson.hpp"
#include "observer.hpp"
#include "Light.hpp"

//! Conversion for units for Cosine and Sine
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

//! texture must be put here to avoid looading and seg fault issues
unsigned int texture[6];

#define LEN 8192

/**
 * The Display Function
 * The primary function to display objects.
 * This is called on a per frame basis to
 *   draw everything
 */
void Display(){
    //! logistical items for proper loading
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    FirstPerson::changeView();
    Flyweight* fly=new Flyweight(texture);
    fly->drawItems();

    //! Ground and Skybox 
    Shapes* g=new Ground;
    Shapes* s=new Sky;
    g->draw(0,0,0.02,30,180,texture);
    s->draw(0,0,-0.02,40,180,texture);

    //! Lighting call to make light and fog occur
    Light* lt=new Light;
    lt->makeLight();
    lt->fog();

    //! Error check for OpenGL
    ErrCheck("Display");

    //! flush and switch to leave it open to another call to Display
    glFlush();
    glutSwapBuffers();
} 

/**
 * The Main Function.
 * This uses Glut listeners and initializers to
 * load the project as well as loads textures.
 */
int main(int argc, char* argv[]){
    //! window and GLUT intializer
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("Final Project: Luna McBride");

    //! Loading textures into the texture array
    texture[0]=LoadTexBMP("grass.bmp"); 
    texture[1]=LoadTexBMP("pine.bmp"); 
    texture[2]=LoadTexBMP("bark2.bmp");
    texture[3]=LoadTexBMP("mushroomBottom.bmp");
    texture[4]=LoadTexBMP("mushroomTop.bmp");
    texture[5]=LoadTexBMP("gsky.bmp");

    //! GLUT listeners, using an Observer to put into classes
    Observer* ob=new Observer;
    glutDisplayFunc(Display);
    glutReshapeFunc(ob->notify);
    glutSpecialFunc(ob->notify);
    glutKeyboardFunc(ob->notify);

    //! Error checking and looping before ending
    ErrCheck("main");
    glutMainLoop();
    return 0;
}