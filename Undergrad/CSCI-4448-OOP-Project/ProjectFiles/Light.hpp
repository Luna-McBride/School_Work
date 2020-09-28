#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <string>
#include "shapes.hpp"

/**
 * The Light Class.
 * This draws a light source and fog
 *
 */

class Light{
	public:
		~Light(){
		
		}
		//! Applies the lighting shown in Graphics class to objects
		void makeLight(){
			/*! Calculates Ambient, Diffuse, Specular, and Position
			of a light source, which I intended to mess with but did not have the time*/ 
		    float Ambient[]={((float)0.01)*ambient ,((float)0.01)*ambient ,((float)0.01)*ambient ,1.0};
		    float Diffuse[]={((float)0.01)*diffuse ,((float)0.01)*diffuse ,((float)0.01)*diffuse ,1.0};
		    float Specular[]={((float)0.01)*specular,((float)0.01)*specular,((float)0.01)*specular,1.0};
		    float Position[]  = {X+(5*(float)Cos(zh)),Y+ylight,Z+(5*(float)Sin(zh)),1};
		      
		    //! Draws a Sphere to represent a light source
		    glColor3f(1,1,1);
		    sp->draw(Position[0],Position[1],Position[2] , 0.1,0,c);
		      
		    //! Enables the proper OpenGL items for lighting
		    glEnable(GL_NORMALIZE);
		    glEnable(GL_LIGHTING);  
		    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		    glEnable(GL_COLOR_MATERIAL);
		    glEnable(GL_LIGHT0);
		      
		    //! Applies the calculated items to the GL enabled items
		    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
		    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
		    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
		    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
		    glLightfv(GL_LIGHT0,GL_POSITION,Position);
		}
		//! makes a fog across the area
		void fog(){
			glEnable(GL_FOG);
	  		glFogf(GL_FOG_MODE,GL_EXP);
	  		glFogf(GL_FOG_DENSITY,0.05);
	  		float col[4]={0.7,0.7,0.7,0.5};
	  		glFogfv(GL_FOG_COLOR,col);
		    glutPostRedisplay();
		}
		
	private:
		//! XYZ values for movement
		float X=0;
		float Y=0;
		float Z=10;

		//! calculations for lighting and the light source
		int emission=0;
		float ambient=30;
		int diffuse=70;
		float specular=0;
		int shininess=0;
		float shiny=1;
		int zh=90;
		float ylight=0;
		int side=1;
		Sphere* sp=new Sphere;

		//! false item to take the place of the texture array for calling the Sphere
		unsigned int c[2]={0};
};

#endif