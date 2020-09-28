#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <string>

/**
 * The Shapes Class.
 * A container for the draw function and
 *  a vertex system for sphere-like objects.
 */
class Shapes{
	public:
		static void Vertex(double theta, double pheta){
			glVertex3d(Sin(theta)*Cos(pheta), Sin(pheta), Cos(theta)*Cos(pheta));
		}
		virtual void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){}
		
};

/**
 * The Cyl Class.
 * Draws a cylender, though I cannot spell cylendar for the life of me
 */
class Cyl:virtual public Shapes{
	public:
		//! Draws the Cyl given location, scale, and rotation
		void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){
		   
		   glPushMatrix();
		   glTranslated(x,y,z);
		   glScaled(scale,scale,scale);
		   glRotated(rotation,1,0,0);

		   for (int i=1;i>=-1;i-=2){
		      glNormal3f(0,0,i);
		      glBegin(GL_TRIANGLE_FAN);
		      glTexCoord2f(0.5,0.5);
		      glVertex3f(0,0,i);
		      for (int k=0;k<=360;k+=10)
		      {
		         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
		         glVertex3f(i*Cos(k),Sin(k),i);
		      }
		      glEnd();
		   }
		   
		   glBegin(GL_QUAD_STRIP);
		   for (int k=0;k<=360;k+=10){
		      glNormal3f(Cos(k),Sin(k),0);
		      glTexCoord2f(0.4,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
		      glTexCoord2f(0.6,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
		   }
		   glEnd();
		   
		   glPopMatrix();
		   ErrCheck("cyl");
		}
};

/**
 * The Circle Class.
 * Draws a circle given location, scale, and rotation
 *
 */
class Circle:virtual public Shapes{
	public:
		//! Draws the Circle given location, scale, and rotation
		void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){
			int k;
		    glPushMatrix();
		    glTranslated(x,y,z);
		    glScaled(scale,scale,scale);
		    glRotated(rotation,1,0,0);
		    
		    glBegin(GL_TRIANGLE_FAN);
		    glNormal3f(0,0,0.25);
			glTexCoord2f(0.5,0.5); glVertex3f(0,0,0);
			for (k=0;k<=360;k+=10){
				glTexCoord2f(0.65/2*Cos(k)+0.5,0.7/2*Sin(k)+0.5);
				glVertex2f(Cos(k),Sin(k));
			}
			glEnd();

		    glPopMatrix();
		    ErrCheck("circle");
		}
};

/**
 * The Cone Class.
 * Draws a cone (or tree top) given a location, scale, and rotation
 */
class Cone:virtual public Shapes{
	public:
		//! Draws the Cone given location, scale, and rotation
		void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){
			int d=10;
		    glPushMatrix();
		    glTranslated(x,y,z);
		    glScaled(scale,scale,scale);
		    glRotated(90,1,0,0);
		    glEnable(GL_TEXTURE_2D);
  			glBindTexture(GL_TEXTURE_2D,texture[1]);
		    
		    glBegin(GL_TRIANGLE_STRIP);
		    for (int i=0;i<=360;i+=d){
		    	glTexCoord2f(6/2*Cos(i)+0.5,6/2*Sin(i)+0.5);
		    	glNormal3f(-Sin(i)*Cos(0), -Sin(0), -(Cos(i)*Cos(0)));
		        Vertex(i,0);
		        glTexCoord2f(0,2);
		        glVertex3d(0,2,0);
		    }
		    
		    glNormal3f(0,-1,0); glVertex3d(0,2,0);
		    glEnd();
		    Circle* cir=new Circle;
		    cir->draw(0,0,0,1,90,texture);
		    glDisable(GL_TEXTURE_2D);
		   
		    glRotated(90,1,0,0);
		    glPopMatrix();
		    ErrCheck("cone");
		}
};

/**
 * The Dome Class.
 * Draws a dome (mushroom top) given a location, scale, and rotation
 */
class Dome:virtual public Shapes{
	public:
		//! Draws the Dome given location, scale, and rotation
		void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){
			const int d=5;
		    int th,p;

		    glPushMatrix();
		    glTranslated(x,y,z);
		    glScaled(scale,scale,scale);
		    glRotated(90,1,0,0);

		    glEnable(GL_TEXTURE_2D);
  			glBindTexture(GL_TEXTURE_2D,texture[4]);

		    for(p=0;p<90;p+=d){
		        glBegin(GL_QUAD_STRIP);
		        for(th=0;th<=360;th+=d){
		        	glNormal3f(-Sin(th)*Cos(p), -Sin(p), -Cos(th)*Cos(p));
		            Vertex(th,p);
		            Vertex(th,p+d);
		        }
		        glEnd();
		    }
		    glDisable(GL_TEXTURE_2D);
		    glEnable(GL_TEXTURE_2D);
		    glBindTexture(GL_TEXTURE_2D,texture[3]);
		    Circle* cir=new Circle;
		    glDisable(GL_TEXTURE_2D);
		    cir->draw(0,0,0,1,90,texture);
		    
		    glPopMatrix();
		    ErrCheck("dome");
		}
};

/**
 * The Sky Class.
 * Draws a skybox given the location of the box, scale, and rotation
 *
 */
class Sky: virtual public Shapes{
	public:
		//! Draws the Sky given location, scale, and rotation
		void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){
			int p=1;

		    glPushMatrix();
		    
		    glTranslated(x,y,z);
		    glRotated(rotation,0,1,0);
		    glScaled(scale,scale,scale);
		    glColor3f(1,1,1);
		    glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,texture[5]);
		    
		    glBegin(GL_QUADS);
		    //  Front
		   glNormal3f(0,0,1);
		   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
		   glTexCoord2f(p,0); glVertex3f(+1,-1, 1);
		   glTexCoord2f(p,p); glVertex3f(+1,+1, 1);
		   glTexCoord2f(0,p); glVertex3f(-1,+1, 1);
		   glEnd();
		   //  Back
		   glNormal3f(0,0,-1);
		   glBegin(GL_QUADS);
		   glTexCoord2f(p,0); glVertex3f(+1,-1,-1);
		   glTexCoord2f(p,p); glVertex3f(-1,-1,-1);
		   glTexCoord2f(0,p); glVertex3f(-1,+1,-1);
		   glTexCoord2f(0,0); glVertex3f(+1,+1,-1);
		   glEnd();
		   //  Right
		   glNormal3f(1,0,0);
		   glBegin(GL_QUADS);
		   glTexCoord2f(0,p); glVertex3f(+1,-1,+1);
		   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
		   glTexCoord2f(p,0); glVertex3f(+1,+1,-1);
		   glTexCoord2f(p,p); glVertex3f(+1,+1,+1);
		   glEnd();
		   //  Left
		   glNormal3f(-1,0,0);
		   glBegin(GL_QUADS);
		   glTexCoord2f(p,0); glVertex3f(-1,-1,-1);
		   glTexCoord2f(p,p); glVertex3f(-1,-1,+1);
		   glTexCoord2f(0,p); glVertex3f(-1,+1,+1);
		   glTexCoord2f(0,0); glVertex3f(-1,+1,-1);
		   glEnd();
		   //  Top
		   glNormal3f(0,1,0);
		   glBegin(GL_QUADS);
		   glTexCoord2f(p,p); glVertex3f(-1,+1,+1);
		   glTexCoord2f(0,p); glVertex3f(+1,+1,+1);
		   glTexCoord2f(0,0); glVertex3f(+1,+1,-1);
		   glTexCoord2f(p,0); glVertex3f(-1,+1,-1);
		   glEnd();
		   //  Bottom
		   glNormal3f(0,-1,0);
		   glBegin(GL_QUADS);
		   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
		   glTexCoord2f(p,0); glVertex3f(+1,-1,-1);
		   glTexCoord2f(p,p); glVertex3f(+1,-1,+1);
		   glTexCoord2f(0,p); glVertex3f(-1,-1,+1);
		   
		   glEnd();
		   glDisable(GL_TEXTURE_2D);
		   
		   glPopMatrix();
		   ErrCheck("sky");
		}
};

/**
 * The Ground Class
 * Draws a grassy ground following everything the shapes 
 * draw function needs
 */
class Ground: virtual public Shapes{
	public: 
		//! Draws the Ground given location, scale, and rotation
		void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){
			int num=90;
			double rep=5;
			double mul = 2.0/num;
			glPushMatrix();
			glTranslated(x,y,z);
			glRotated(rotation,0,1,0);
			glScaled(scale,scale,scale);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,texture[0]);

			glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);
			glColor3f(1,1,1);
			for (int i=0;i<num;i++){
			  for (int j=0;j<num;j++){
			     glTexCoord2d(mul*(i+0),mul*(j+0)); glVertex2d(5*mul*(i+0)-5,5*mul*(j+0)-5);
			     glTexCoord2d(mul*(i+1),mul*(j+0)+rep); glVertex2d(5*mul*(i+1)-5,5*mul*(j+0)-5);
			     glTexCoord2d(mul*(i+1)+rep,mul*(j+1)+rep); glVertex2d(5*mul*(i+1)-5,5*mul*(j+1)-5);
			     glTexCoord2d(mul*(i+0)+rep,mul*(j+1)); glVertex2d(5*mul*(i+0)-5,5*mul*(j+1)-5);
			  }
			}
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	private:
		int num=90;
		int rep=5;
		double mul=2/num;
};

/**
 * The Sphere Class.
 * Draws a sphere given the location, scale, and rotation
 *
 */
class Sphere:virtual public Shapes{
	public:
		//! Draws the Sphere given location, scale, and rotation
		void draw(double x, double y, double z, double scale, double rotation, unsigned int *texture){
		    const int d=5;

		    glPushMatrix();
		    glTranslated(x,y,z);
		    glScaled(scale,scale,scale);
		    glRotated(rotation,1,0,0);

		    for(int p=-90;p<90;p+=d){
		        glBegin(GL_QUAD_STRIP);
		        for(int t=0;t<=360;t+=d){
		        	glNormal3f(Sin(t)*Cos(p), Sin(p), Cos(t)*Cos(p));
		            Vertex(t,p);
		            Vertex(t,p+d);
		        }
		        glEnd();
		    }

		    glPopMatrix();   
		}
};
  

#endif