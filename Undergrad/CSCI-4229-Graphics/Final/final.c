#include "CSCIx229.h"

//mostly taken from various Examples
int arr[20]={0};
int pType=2;
int id=1;
int mode=9;
int test=0;
int th=0;
int ph=0;
int side=1;
int fov=55;
int t;
double asp=1;
double dim=6.0;
double dt=0.1;
double dx=1;
double dy=0;
double dz=0;
double Ex=-15;
double Ey=0;
double Esx=-15;
double Esy=0;
double Ez=1;
double rep=5;
double reps;
int num=90;
unsigned int texture[10];
//taken from example 15
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
//concept taken from Ex21
float X=0;
float Y=0;
float Z=0;
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

//taken from ex15. Counts time and provides a wait for animations
void idle()
{
        //  Elapsed time in seconds
	   double c = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	   zh = fmod(90*c,360.0);
     t=t+1;

	   //  Tell GLUT it is necessary to redisplay the scene
	   glutPostRedisplay();
	   ErrCheck("Idle");
}

//  Skybox (Altered from Example 8, combined with Ex14, Ex16, and Ex21)
static void ground(double x,double y,double z, double dx,double dy,double dz, double th){
	int i,j;
	double mul = 2.0/num;
	//  Save transformation
	glPushMatrix();
	//  Offset
	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);
	reps=rep;
	rep=5;
	glEnable(GL_TEXTURE_2D);
	//glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glColor3f(1,1,1);
	//does not look good on regular perspective size, looks good on First Person (mainly emphasized camera)
	for (i=0;i<num;i++){
	  for (j=0;j<num;j++){
	     glTexCoord2d(mul*(i+0),mul*(j+0)); glVertex2d(5*mul*(i+0)-5,5*mul*(j+0)-5);
	     glTexCoord2d(mul*(i+1),mul*(j+0)+rep); glVertex2d(5*mul*(i+1)-5,5*mul*(j+0)-5);
	     glTexCoord2d(mul*(i+1)+rep,mul*(j+1)+rep); glVertex2d(5*mul*(i+1)-5,5*mul*(j+1)-5);
	     glTexCoord2d(mul*(i+0)+rep,mul*(j+1)); glVertex2d(5*mul*(i+0)-5,5*mul*(j+1)-5);
	  }
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
  //actually draws the squares that make up the bigger ground
	glBegin(GL_LINES);
	for (i=0;i<=num;i++){
	 glVertex3d(5*mul*i-5,-5,0.01);
	 glVertex3d(5*mul*i-5,+5,0.01);
	}
	for (j=0;j<=num;j++){
	 glVertex3d(-5,5*mul*j-5,0.01);
	 glVertex3d(+5,5*mul*j-5,0.01);
	}
	glEnd();
	rep=reps;
	//  Undo transformations
	glPopMatrix();
}

//taken from Example 8 (more to mess with camera and placement currently
static void Vertex(double t, double p){
    glVertex3d(Sin(t)*Cos(p), Sin(p), Cos(t)*Cos(p));
}

//taken from Example 8 and modified for the dango and light sphere
static void sphere(double x, double y, double z, double r, double p1, double p2, double p3){
    const int d=5;
    int th,p;

    //hold onto current transformation state
    glPushMatrix();
    glColor3f(p1,p2,p3);
    //move and scale
    glTranslated(x,y,z);
    glScaled(r,r,r);

    //bands
    for(p=-90;p<90;p+=d){
        glBegin(GL_QUAD_STRIP);
        for(th=0;th<=360;th+=d){
        	glNormal3f(-Sin(th)*Cos(p), -Sin(p), -Cos(th)*Cos(p));
            Vertex(th,p);
            Vertex(th,p+d);
        }
        glEnd();
    }

    //load save state
    glPopMatrix();
    ErrCheck("sphere1");   
}

//An additional sphere added to make up for some oddities
static void sphere2(double x, double y, double z, double r, double p1, double p2, double p3){
    const int d=5;
    int th,p;

    //hold onto current transformation state
    glPushMatrix();
    glColor3f(p1,p2,p3);
    //move and scale
    glTranslated(x,y,z);
    glScaled(r,r,r);

    //bands
    for(p=-90;p<90;p+=d){
        glBegin(GL_QUAD_STRIP);
        for(th=0;th<=360;th+=d){
          glNormal3f(Sin(th)*Cos(p), Sin(p), Cos(th)*Cos(p));
            Vertex(th,p);
            Vertex(th,p+d);
        }
        glEnd();
    }

    //load save state
    glPopMatrix();
    ErrCheck("sphere");   
}
//Using the Example 15 cube, this creates a skybox
static void sky(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){
    int p=1;
    //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D,texture[1]);
   //  Cube
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
   //  End
   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transformations
   glPopMatrix();
   ErrCheck("sky");
}

//a regular cube for building the torii and other usage. Taken from Example 15
static void cube(double x,double y,double z, double dx,double dy,double dz,double th, double p1, double p2, double p3, double r){
    int p=1;
    //  Save transformation
   glPushMatrix();
   //  Offset
   glColor3f(p1,p2,p3);
   glTranslated(x,y,z);
   if(th>50){
     glRotated(th,0,0,r);
   }
   else{
     glRotated(th,0,r,0);
   }
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front (No need for textures on front or back)
   glNormal3f(0,0,1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glNormal3f(0,0,-1);
   glBegin(GL_QUADS);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
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
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   ErrCheck("cube");
}

//modified from Ex19's coin. A cylender for the torii
static void cyl(double x,double y,double z,double rx,double ry, double rz, int form,double p1, double p2, double p3)
{
   int i,k;
   float f=1;
   if(form==1){
        f=0.6;
    }
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(rx,ry,rz);
   //  Head & Tail
   glColor3f(p1,p2,p3);
   for (i=1;i>=-1;i-=2){
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(f*i*Cos(k),f*Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10){
      if(f!=1){
        glNormal3f(Cos(k)*Cos(0),Sin(k)*Cos(0),Sin(0));
      }
      else{
        glNormal3f(Cos(k),Sin(k),0);
      }
      glTexCoord2f(0,0.5*k); glVertex3f(f*Cos(k),f*Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   ErrCheck("cyl");
}

//Draws a circle. Replaced old one with Example 14's for lighting purposes
static void circle(double x, double y, double z, double r, double ro){
    int k;
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(ro,1,0,0);
    //makes the lighting two sided
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
    //draws the circle and normals
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,0.25);
	  glTexCoord2f(0.5,0.5); glVertex3f(0,0,0);
	  for (k=0;k<=360;k+=10){
		  glTexCoord2f(0.65/2*Cos(k)+0.5,0.7/2*Sin(k)+0.5);
		  glVertex2f(Cos(k),Sin(k));
	  }
	  glEnd();
    //cancels the two sided lighting for everything else
	  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    glPopMatrix();
    ErrCheck("circle");
}


//draws a cone that was used as a tree previously, but kept to test
static void cone(double x, double y, double z, double sx, double sy,double sz, double p1, double p2, double p3){
    int i,d=10;
    glColor3f(p1,p2,p3);
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(sx,sy,sz);
    glRotated(90,1,0,0);
    //creates the opening on the bottom compared to the top point
    glBegin(GL_TRIANGLE_STRIP);
    for (i=0;i<=360;i+=d){
    	glTexCoord2f(6/2*Cos(i)+0.5,6/2*Sin(i)+0.5);
    	glNormal3f(-Sin(i)*Cos(0), -Sin(0), -(Cos(i)*Cos(0)));
        Vertex(i,0);
        glTexCoord2f(0,2);
        glVertex3d(0,2,0);
    }
    //ensure the regularity at the top
    glNormal3f(0,-1,0); glVertex3d(0,2,0);
    glEnd();
    //draws a circle for the bottom
    circle(0,0,0,1,90);
    glRotated(90,1,0,0);
    glPopMatrix();
    ErrCheck("cone");
}




//Taken directly from Example 15 and Example 21(spotlight)
void light(int f){
	//  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[4];
      if(f==0){
        Position[0]  = X+5*Cos(zh);
        Position[1]  = Y+ylight;
        Position[2]  = Z+5*Sin(zh);
        Position[3]  = 1;
      }
      else{
      	Position[0]  = 0;
        Position[1]  = 0;
        Position[2]  = 8;
        Position[3]  = 1;
      }
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
        sphere(Position[0],Position[1],Position[2] , 0.1,1,1,1);
        //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      ErrCheck("light");
      
}

//a function representing the center target and the dango eyes
static void target(double x, double y, double z, double s, double rx, double ry, double rz, int b, int i){
    //makes sure the center target does not act like an eye
    if(b==36){
        glPushMatrix();
        glTranslated(x,y,z);
        glScaled(s,s,s);
        glRotated(90,rx,ry,rz);
        //enables the offset
        glEnable(GL_POLYGON_OFFSET_FILL);
        //backmost red circle of the eye/target
        glColor3f(0.625,0.32,0.176);
        glRotated(0.1,1,0,0);
        glPolygonOffset(2,2);
        circle(0,0,0,1.1,0);
        //middle white circle
        glColor3f(1,1,1);
        glPolygonOffset(1,1);
        circle(0,0,0,1,0);
        glPolygonOffset(0,0);
        //inner black circle
        glColor3f(0,0,0);
        circle(0,0,0,0.1,0);
        //disables the offset
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPopMatrix();
    }
    else{
      //A switch to try to account for the lag
      int q;
      if(pType==3){
      	q=5;
      }
      else{
      	q=10;
      }

      //uses an array of separate dango and changes
      //by time what frame should be used in blinking
      if(t%b==0 && arr[i]==0){
         arr[i]=1;
       }
       if(t%b==q && arr[i]==1){
        arr[i]=2;
       }
       if(t%b==(q*2) && arr[i]==2){
        arr[i]=3;
       }
       if(t%b==(q*3) && arr[i]==3){
        arr[i]=0;
       }

      glPushMatrix();
      //draws the open eye
      if(arr[i]==0){
          glTranslated(x,y,z);
          glScaled(s,s,s);
          glRotated(90,rx,ry,rz);
          //starts to offset the circles so they do not clash
          glEnable(GL_POLYGON_OFFSET_FILL);
          //starts drawing the backmost red circle
          glColor3f(0.625,0.32,0.176);
          glRotated(1,1,0,0);
          glPolygonOffset(2,2);
          circle(0,0,0,1.1,0);
          //starts drawing the middle white circle
          glColor3f(1,1,1);
          glPolygonOffset(1,1);
          circle(0,0,0,1,0);
          //starts the inner pupil
          glPolygonOffset(0,0);
          glColor3f(0,0,0);
          circle(0,0,0,0.3,0);
          //stops the offset
          glDisable(GL_POLYGON_OFFSET_FILL);
          glPopMatrix();
      }
      //follows the same process as the target in the upper one
      //just scaled by 0.5 in the z
      else if(arr[i]==1 || arr[i]==3){
          glTranslated(x,y,z);
          glScaled(s,s,s*0.5);
          glRotated(90,rx,ry,rz);
          glEnable(GL_POLYGON_OFFSET_FILL);
          glColor3f(0.625,0.32,0.176);
          glRotated(1,1,0,0);
          glPolygonOffset(2,2);
          circle(0,0,0,1.1,0);
          glColor3f(1,1,1);
          glPolygonOffset(1,1);
          circle(0,0,0,1,0);
          glPolygonOffset(0,0);
          glColor3f(0,0,0);
          circle(0,0,0,0.3,0);
          glDisable(GL_POLYGON_OFFSET_FILL);
          glPopMatrix();
      }
      //closed eyes
      else{
          glTranslated(x,y,z-0.1);
          glScaled(s,1,s);
          glRotated(90,rx,ry,rz);
          //draws a black box to create closed eyes
          glColor3f(0,0,0);
          glBegin(GL_QUADS);
          glVertex3f(-1,0.25,0);
          glVertex3f(1,0.25,0);
          glVertex3f(1,0,0);
          glVertex3f(-1,0,0);
          glEnd();
          glPopMatrix();
      }
    }  
    ErrCheck("target");
}

//Spheres made to look like hanami dango.
//eyes seen from behind to emulate a cartoony/N64 style for them
static void dango(double x, double y, double z, double s, double an, double r, double p1, double p2, double p3, int b, int i){
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(s,s,s);
    glRotated(an,0,0,r);
    //body of the dango
    if(i==0){
      sphere(0,0,0,1,p1,p2,p3);
    }
    else{
      sphere2(0,0,0,1,p1,p2,p3);
    }    
    //eyes of these dango
    target(0.5,1,0.5,0.4,1,0,0,b,i);
    target(-0.5,1,0.5,0.4,1,0,0,b,i);
    glPopMatrix();
    ErrCheck("dango");
}

//draws the triangle for the small decorations on the torii's lower bar
static void decorTri(double x, double y, double z, double s, double rz, double p1, double p2, double p3){
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(s,0.5*s,0.3*s);
  glRotated(180,0,0,rz);
  glColor3f(p1,p2,p3);
  //draws the hypotenuse side to make this a prism
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0.5,0,0.5);
  glVertex3f(1,-1,0);
  glVertex3f(1,1,0);
  glVertex3f(0,-1,1);
  glVertex3f(0,1,1);
  glEnd();
  //draws the non-hypotenuse sides to make this a prism
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glVertex3f(0,1,0);
  glVertex3f(0,-1,0);
  glVertex3f(1,1,0);
  glVertex3f(1,-1,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(-1,0,0);
  glVertex3f(0,1,0);
  glVertex3f(0,-1,0);
  glVertex3f(0,1,1);
  glVertex3f(0,-1,1);
  glEnd();
  //draws the two triangles on each side
  glBegin(GL_TRIANGLES);
  glNormal3f(0,-1,0);
  glVertex3f(0,1,0);
  glVertex3f(1,1,0);
  glVertex3f(0,1,1);
  glEnd();
  glBegin(GL_TRIANGLES);
  glNormal3f(0,-1,0);
  glVertex3f(0,-1,0);
  glVertex3f(1,-1,0);
  glVertex3f(0,-1,1);
  glEnd();
  glPopMatrix();
  ErrCheck("decor");
}

//draws the gate (torii)
static void torii(double x, double y, double z, double r, double rz){
    glPushMatrix();
    glTranslated(x,y,z+5);
    glScaled(r,r,r);
    glRotated(90,0,0,rz);
    //draws the cylenders up the gate
    cyl(3,0,-4.5,0.5,0.5,0.5,0,0.5,0.5,0.5);
    cyl(3,0,-1.5,0.5,0.5,3,1,0.859,0.078,0.234);
    cyl(-3,0,-4.5,0.5,0.5,0.5,0,0.5,0.5,0.5);
    cyl(-3,0,-1.5,0.5,0.5,3,1,0.859,0.078,0.234);
    //the lower bar
    cube(0,0,0.5,4.5,0.201,0.3,0,1,0,0,0);
    //red part of the top bar
    cube(0,0,1.8,3.5,0.3,0.3,0,1,0,0,0);
    cube(4,0,1.9,1,0.3,0.3,-5,1,0,0,1);
    cube(-4,0,1.9,1,0.3,0.3,5,1,0,0,1);
    //The black part immediately above the red top bar
    cube(0,0,2.4,3.3,0.3,0.3,0,0.5,0.5,0.5,0);
    cube(4,0,2.5,1.2,0.3,0.3,-4.5,0.5,0.5,0.5,1);
    cube(-4,0,2.5,1.2,0.3,0.3,4.5,0.5,0.5,0.5,1);
    //The further extended part at the very top
    cube(0,0,2.7,3.3,0.4,0.1,0,0.41,0.41,0.41,0);
    cube(4,0,2.8,1.2,0.4,0.1,-3,0.41,0.41,0.41,1);
    cube(-4,0,2.8,1.2,0.4,0.1,3,0.41,0.41,0.41,1);
    //The small triangle decorations on the lower bar
    decorTri(-3.5,0,0.9,0.4,0,1,0,0);
    cube(-2.7,0,0.7,0.2,0.2,0.2,0,1,0,0,0);
    decorTri(-2.5,0,0.9,0.4,1,1,0,0);
    cube(-3.3,0,0.7,0.2,0.2,0.2,0,1,0,0,0);
    decorTri(2.5,0,0.9,0.4,0,1,0,0);
    cube(2.7,0,0.8,0.2,0.2,0.1,0,1,0,0,0);
    decorTri(3.5,0,0.9,0.4,1,1,0,0);
    cube(3.3,0,0.8,0.2,0.2,0.1,0,1,0,0,0);
    glPopMatrix();
    ErrCheck("torii");
}

//A trapezoid used for the rooves
static void trape(double x, double y, double z, double sx, double s, double rz, double p1, double p2, double p3, double pi, double big, int top){
    glPushMatrix();
    glTranslated(x,y,z);
    //A switch to alter the scale depending on if they
    //are an upper big one, small one, or at the top
    if((big==1 && top==1) && (rz==0 || rz>=2)){
      glScaled(s,1*sx,s*100);
    }
    else if(rz==0 || rz>=2){
      glScaled(s,1*sx,s*1.5);
    }
    else if((big==1 && top==1)){
      glScaled(s,1*sx,s*2);
    }
    else{
      glScaled(1*sx,s,s*1.5);
    }
    //Rotations and normals for different directions
    if(rz>=2){
      glRotated(180,0,0,1);
      glNormal3f(0.5,0,0.25);
    }
    else if(rz==0){
      glNormal3f(-0.5,0,0.25);
    }
    else if(rz==-1){
      glRotated(90,0,0,rz);
      glNormal3f(-1,1,-1);
    }
    else{
      glRotated(90,0,0,rz);
      glNormal3f(-1,-1,-1);
    }
    glRotated(pi,0,1,0);
    glColor3f(p1,p2,p3);
    //draws the item depending on what part of the roof it is
    if(big==0){
      glBegin(GL_QUADS);
      glTexCoord2f(1,0); glVertex3d(0,1.51,0);
      glTexCoord2f(0,0); glVertex3d(0,-1.51,0);
      glTexCoord2f(0.2,0.8); glVertex3d(0,-0.9,1);
      glTexCoord2f(0.8,0.8); glVertex3d(0,0.9,1);
      glEnd();
      glPopMatrix();
    }
    else if(big==1 && top==1){
      glBegin(GL_QUADS);
      glTexCoord2f(1,0); glVertex3d(0,1.45,0);
      glTexCoord2f(0,0); glVertex3d(0,-1.45,0);
      glTexCoord2f(0.375,0.625); glVertex3d(0,-0.36,1);
      glTexCoord2f(0.625,0.625); glVertex3d(0,0.36,1);
      glEnd();
      glPopMatrix();
    }
    else{
      glBegin(GL_QUADS);
      glTexCoord2f(1,0); glVertex3d(0,1.45,0);
      glTexCoord2f(0,0); glVertex3d(0,-1.45,0);
      glTexCoord2f(0.375,0625); glVertex3d(0,-0.4,1);
      glTexCoord2f(0.625,0.625); glVertex3d(0,0.4,1);
      glEnd();
      glPopMatrix();
    }
    ErrCheck("trapezoid");
}

//A square used for the bottom part of rooves and wood platforms that separates parts
static void square(double x, double y, double z, double sx, double sy, double sz, double p1, double p2, double p3,double rx){
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(sx,sy,sz);
    glRotated(180,0,0,rx);
    glNormal3f(0,0,1);
    glColor3f(p1,p2,p3);
    //draws square
    glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex3d(-1,1,0);
    glTexCoord2f(0,0); glVertex3d(-1,-1,0);
    glTexCoord2f(1,0); glVertex3d(1,-1,0);
    glTexCoord2f(1,1); glVertex3d(1,1,0);
    glEnd();
    glPopMatrix();
    ErrCheck("square");
}

//Basically the cyl function but just the top loops
static void loop(double x,double y,double z,double rx,double ry, double rz, double p1, double p2, double p3)
{
   int k;
   float f=1;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(rx,ry,rz);
   glColor3f(p1,p2,p3);
   //  Edge
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10){
      if(f!=1){
        glNormal3f(Cos(k)*Cos(0),Sin(k)*Cos(0),Sin(0));
      }
      else{
        glNormal3f(Cos(k),Sin(k),0);
      }
      glTexCoord2f(0,0.5*k); glVertex3f(f*Cos(k),f*Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   ErrCheck("loop");
}

//Draws the base for the pagoda
static void pagodaBase(double x, double y, double z, double s,int top){
     double i;
     //Checks if this is supposed to be a roof
     if(top==0){
       glPushMatrix();
       glTranslated(x,y,z);
       glScaled(s,s,s);
       //Draws the cube base with the front texture
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[3]);
       cube(0,0,0,2,1.5,1,0,1,1,1,0);
       glDisable(GL_TEXTURE_2D);
       //Draws the roof
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[5]);
       trape(2.6,0,0.8,1.35,1,0,0.2,0.2,0.2,-71,0,1);
       trape(-2.6,0,0.8,1.35,1,3,0.2,0.2,0.2,-71,0,1);
       trape(0,2.05,0.79,1.73,1,1,0.2,0.2,0.2,-68,0,1);
       trape(0,-2.05,0.79,1.73,1,-1,0.2,0.2,0.2,-68,0,1);
       trape(2.9,0,1.75,1.7,2,0,0.2,0.2,0.2,-89.71,1,1);
       trape(0,2.45,1.75,2.1,2,1,0.2,0.2,0.2,-73.7,1,1);
       trape(-2.9,0,1.75,1.7,2,2,0.2,0.2,0.2,-89.71,1,1);
       trape(0,-2.45,1.75,2.1,2,-1,0.2,0.2,0.2,-73.7,1,1);
       glDisable(GL_TEXTURE_2D);
       //Draws and textures the weird part circled by the rooves
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[4]);
       cube(0,0,1.5,1.7,1.2,1.1,0,1,1,1,0);
       glDisable(GL_TEXTURE_2D);
       glEnable(GL_TEXTURE_2D);
       //Draws the lower parts of the rooves and top connector
       glBindTexture(GL_TEXTURE_2D,texture[6]);
       square(0,0,1.75,2.9,2.45,2.8,0.1,0.1,0.1,0);
       square(0,0,0.8,2.65,2.1,2.8,0.1,0.1,0.1,0);
       square(0,0,2.62,2,1.5,1,0.4,0.4,0.4,0);
       glDisable(GL_TEXTURE_2D);
       glPopMatrix();
     }
     else {
       glPushMatrix();
       glTranslated(x,y,z);
       glScaled(s,s,s);
       //Draws the base cube with the door-like texture
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[3]);
       cube(0,0,0,2,1.5,1,0,1,1,1,0);
       glDisable(GL_TEXTURE_2D);
       //Draws the weird central cube surrounded by the rooves
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[4]);
       cube(0,0,1.5,1.7,1.2,0.7,0,1,1,1,0);
       glDisable(GL_TEXTURE_2D);
       //Draws the roof trapezoids
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[5]);
       trape(2.6,0,0.8,1.35,1,0,0.2,0.2,0.2,-71,0,1);
       trape(-2.6,0,0.8,1.35,1,3,0.2,0.2,0.2,-71,0,1);
       trape(0,2.05,0.79,1.73,1,1,0.2,0.2,0.2,-68,0,1);
       trape(0,-2.05,0.79,1.73,1,-1,0.2,0.2,0.2,-68,0,1);
       trape(2.9,0,1.75,1.7,2,0,0.2,0.2,0.2,-89.71,1,1);
       trape(0,2.45,1.75,2.1,2,1,0.2,0.2,0.2,-73.7,1,1);
       trape(-2.9,0,1.75,1.7,2,2,0.2,0.2,0.2,-89.71,1,1);
       trape(0,-2.45,1.75,2.1,2,-1,0.2,0.2,0.2,-73.7,1,1);
       glDisable(GL_TEXTURE_2D);
       //Draws the underparts of the rooves
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[6]);
       square(0,0,1.75,2.9,2.45,2.8,0.1,0.1,0.1,0);
       square(0,0,0.8,2.65,2.1,2.8,0.1,0.1,0.1,0);
       glDisable(GL_TEXTURE_2D);
       //Puts a concrete texture and draws some upper parts
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D,texture[7]);
       cube(0,0,3.7,0.25,0.25,0.25,1,0.5,0.5,0.5,0);
       cube(0,0,2.79,1,0.6,0.1,1,0.5,0.5,0.5,0);
       cone(0,0,4,0.2,0.2,3,0.5,0.5,0.5);
       cube(0,0,4,0.32,0.32,0.05,0,0.5,0.5,0.5,0);
       for(i=4.3;i<=6.7;i+=0.3){
         loop(0,0,i,0.3,0.3,0.07,0.5,0.5,0.5);
       }
       glDisable(GL_TEXTURE_2D);
       //Draws the cyan items that look cyan on the tower
       cyl(0,0,3,0.4,0.4,0.45,0,0,1,1);
       cube(0,0,2.95,1,0.6,0.05,0,0,1,1,0);      
       //Draws the decorations on the huge spire
       sphere2(0,0,9,0.15,0.75,0.75,0.75);
       cone(0,0,8,0.2,0.4,0.4,0.75,0.75,0.75);
       
       glPopMatrix();
     }
     ErrCheck("base");
}

//Draws the pagoda from its bases
static void pagoda(double x, double y, double z, double s){
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(s,s,s);
  pagodaBase(0,0,0,1,0);
  pagodaBase(0,0,3.3,0.7,0);
  pagodaBase(0,0,5.55,0.4,1);
  glPopMatrix();
  ErrCheck("pagoda");
}

//loads the area and assets
void area(){
    if(pType!=3){
        //loads object by test value for testing in a perspective view
        glRotated(-90,1,0,0);
        if(test==0){
          light(0);
          dango(0,0,0,1,180,1,0.996,0.75,0.793,210,0);
        }
        //shows off how they blink in different cycles
        else if(test==1){
          dango(0,0,0,1,180,1,0.996,0.75,0.793,210,0);
          dango(0,0,2,1,180,1,0,0.391,0,150,1);
        }
        //Shows a torii
        else if(test==2){
          light(0);
          torii(0,0,-5,1,0);
        }
        //Draws the full pagoda
        else if(test==3){
           light(0);
           pagoda(0,0,-3,1);
        }
        //Shows the scene of the first person in perspective
        else if(test==4){
          double i;
		  //regular assets and light position
		  light(1);
		  ground(0,0,-0.02,30,30,10,180);
		  //A line of gates like several famous areas have
	      torii(4,0,-2.5,0.5,1);
	      torii(2,0,-2.5,0.5,1);
	      torii(0,0,-2.5,0.5,1);
	      torii(-2,0,-2.5,0.5,1);
	      torii(-4,0,-2.5,0.5,1);
	      torii(-6,0,-2.5,0.5,1);
	      torii(-8,0,-2.5,0.5,1);
	      torii(-10,0,-2.5,0.5,1);
	      torii(-12,0,-2.5,0.5,1);
	      torii(-14,0,-2.5,0.5,1);
	      //a single pagoda
	      pagoda(31,0,1,2);
	      //dango circle at start
	      dango(-15,2,0.4,0.4,180,1,0.996,0.75,0.793,210,1);
	      dango(-17,0,0.4,0.4,-90,1,1,1,1,340,2);
	      dango(-16.5,1.5,0.4,0.4,225,1,0.54,0.75,0.793,140,3);
	      dango(-16.5,-1.5,0.4,0.4,-45,1,0.2,1,1,260,4);
	      dango(-15,-2,0.4,0.4,0,0,0,0.391,0,400,5);
	      //dango in the zen gardem
	      dango(7.7,2.9,0.2,0.2,180,1,0.32,0.6,0.75,111,6);
	      //Dango leading up to the temple.
	      //It is intentionally uncommon for them to blink (They are temple guards)
	      dango(14,1,0.5,0.5,90,1,0.62,0.8,0.45,867,7);
	      dango(14,-1,0.5,0.5,90,1,1,0.1,0.1,530,8);
	      dango(17,1,0.5,0.5,90,1,0.12,0.3,0.95,986,9);
	      dango(17,-1,0.5,0.5,90,1,0.22,0.1,0.7,753,10);
	      dango(20,1,0.5,0.5,90,1,0.22,0.3,0.5,986,11);
	      dango(20,-1,0.5,0.5,90,1,0.92,1,0.92,178,12);
	      dango(23,1,0.5,0.5,90,1,0.22,0.3,0.25,187,13);
	      dango(23,-1,0.5,0.5,90,1,0.92,0.1,0.77,999,14);
	      //Dango in the gates
	      dango(-5,-1.5,0.2,0.2,0,0,0.7,0.6,0.5,56,15);
	      dango(-1,1.5,0.2,0.2,180,1,0.62,0.4,0.1,86,16);
	      dango(3,-1.5,0.2,0.2,0,0,0.22,1,0.22,118,17);
	      dango(-11,1.5,0.2,0.2,180,1,0.62,0.5,0.95,87,18);
	      dango(-9,-1.5,0.2,0.2,0,0,0.72,0.77,0.77,99,19);
	      //creates the cobblestone pathway
	      glEnable(GL_TEXTURE_2D);
	      glBindTexture(GL_TEXTURE_2D,texture[2]);
	      for(i=-13.5;i<=29.5;i+=2){
	        square(i,0,0,1,1,1,1,1,1,0);
	      }
	      glDisable(GL_TEXTURE_2D);
	      //creates the rock/zen garden
	      glEnable(GL_TEXTURE_2D);
	      glBindTexture(GL_TEXTURE_2D,texture[8]);
	      square(7,3,0,2,2,2,1,1,1,0);
	      square(11,3,0,2,2,2,1,1,1,0);
	      square(7,-3,0,2,2,2,1,1,1,1);
	      square(11,-3,0,2,2,2,1,1,1,1);
	      glDisable(GL_TEXTURE_2D);
	      //the blocks surrouding the zen garden and path up to the temple
	      cube(13,3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
	      cube(5,3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
	      cube(7,5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
	      cube(11,5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
	      cube(13,-3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
	      cube(5,-3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
	      cube(7,-5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
	      cube(11,-5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
	      cube(19.9,1,0,0.1,7,0.1,90,0.625,0.32,0.176,1);
	      cube(19.9,-1,0,0.1,7,0.1,90,0.625,0.32,0.176,1);
			  //Center Target
	      target(-15,0,0.02,1,0,0,0,36,36);
        }
        glDisable(GL_LIGHTING);
    }
    else{
      double i;
      //makes the fog
	  glEnable(GL_FOG);
	  glFogf(GL_FOG_MODE,GL_EXP);
	  glFogf(GL_FOG_DENSITY,0.1);
	  float col[4]={0.7,0.7,0.7,0.5};
	  glFogfv(GL_FOG_COLOR,col);
	  //regular assets and light position
	  light(1);
	  //Sky no longer skycube, but backdrop to make fog work
	  sky(0,0,8,40,40,10,180);
	  ground(0,0,-0.02,30,30,10,180);
	  //A line of gates like several famous areas have
      torii(4,0,-2.5,0.5,1);
      torii(2,0,-2.5,0.5,1);
      torii(0,0,-2.5,0.5,1);
      torii(-2,0,-2.5,0.5,1);
      torii(-4,0,-2.5,0.5,1);
      torii(-6,0,-2.5,0.5,1);
      torii(-8,0,-2.5,0.5,1);
      torii(-10,0,-2.5,0.5,1);
      torii(-12,0,-2.5,0.5,1);
      torii(-14,0,-2.5,0.5,1);
      //a single pagoda
      pagoda(31,0,1,2);
      //dango circle at start
      dango(-15,2,0.4,0.4,180,1,0.996,0.75,0.793,210,1);
      dango(-17,0,0.4,0.4,-90,1,1,1,1,340,2);
      dango(-16.5,1.5,0.4,0.4,225,1,0.54,0.75,0.793,140,3);
      dango(-16.5,-1.5,0.4,0.4,-45,1,0.2,1,1,260,4);
      dango(-15,-2,0.4,0.4,0,0,0,0.391,0,400,5);
      //dango in the zen gardem
      dango(7.7,2.9,0.2,0.2,180,1,0.32,0.6,0.75,111,6);
      //Dango leading up to the temple.
      //It is intentionally uncommon for them to blink (They are temple guards)
      dango(14,1,0.5,0.5,90,1,0.62,0.8,0.45,867,7);
      dango(14,-1,0.5,0.5,90,1,1,0.1,0.1,530,8);
      dango(17,1,0.5,0.5,90,1,0.12,0.3,0.95,986,9);
      dango(17,-1,0.5,0.5,90,1,0.22,0.1,0.7,753,10);
      dango(20,1,0.5,0.5,90,1,0.22,0.3,0.5,986,11);
      dango(20,-1,0.5,0.5,90,1,0.92,1,0.92,178,12);
      dango(23,1,0.5,0.5,90,1,0.22,0.3,0.25,187,13);
      dango(23,-1,0.5,0.5,90,1,0.92,0.1,0.77,999,14);
      //Dango in the gates
      dango(-5,-1.5,0.2,0.2,0,0,0.7,0.6,0.5,56,15);
      dango(-1,1.5,0.2,0.2,180,1,0.62,0.4,0.1,86,16);
      dango(3,-1.5,0.2,0.2,0,0,0.22,1,0.22,118,17);
      dango(-11,1.5,0.2,0.2,180,1,0.62,0.5,0.95,87,18);
      dango(-9,-1.5,0.2,0.2,0,0,0.72,0.77,0.77,99,19);
      //creates the cobblestone pathway
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      for(i=-13.5;i<=29.5;i+=2){
        square(i,0,0,1,1,1,1,1,1,0);
      }
      glDisable(GL_TEXTURE_2D);
      //creates the rock/zen garden
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,texture[8]);
      square(7,3,0,2,2,2,1,1,1,0);
      square(11,3,0,2,2,2,1,1,1,0);
      square(7,-3,0,2,2,2,1,1,1,1);
      square(11,-3,0,2,2,2,1,1,1,1);
      glDisable(GL_TEXTURE_2D);
      //the blocks surrouding the zen garden and path up to the temple
      cube(13,3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
      cube(5,3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
      cube(7,5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
      cube(11,5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
      cube(13,-3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
      cube(5,-3,0,0.1,2,0.1,0,0.625,0.32,0.176,0);
      cube(7,-5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
      cube(11,-5,0,0.1,2.1,0.1,90,0.625,0.32,0.176,1);
      cube(19.9,1,0,0.1,7,0.1,90,0.625,0.32,0.176,1);
      cube(19.9,-1,0,0.1,7,0.1,90,0.625,0.32,0.176,1);
		  //Center Target
      target(-15,0,0.02,1,0,0,0,36,36);
      glDisable(GL_FOG);
    }
    ErrCheck("Area");
}

//basically display the objects (taken from HW1, modified with various examples)
void display(){
    const double len=1.5;
    //clear whatever garbage it tries to pull up
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    //ZBUFFER
    glEnable(GL_DEPTH_TEST);
    //clears any random transformations
    glLoadIdentity();
    //sets initial rotation conditions
    if(pType==2){
    	  id=1;
        double E1=-2*dim*Sin(th)*Cos(ph);
        double E2=+2*dim*Sin(ph);
        double E3=+2*dim*Cos(th)*Cos(ph);
        gluLookAt(E1,E2,E3,0,0,0,0,Cos(ph),0);
    }
    //sets the first person
    else if(pType==3){
        gluLookAt(Ex,Ey,Ez,Ex+dx,Ey+dy,Ez+dz,0,0,1);
    }
    
    //loads the objects
    area();


    if(mode==9 && pType!=3){
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
        Print("Angle: %d,%d   Dim: %.1f   Fov: %d ", th, ph, dim,fov);
        if(pType == 2){
            Print("  Projection: Perspective");
        }
        else{
            Print("  Projection: First Person");
        }
    }

    ErrCheck("Display");

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
    else if('2'<=ch && ch<='3'){
        //saves the position of the first person
        if(pType==3){
            Esx=Ex;
            Esy=Ey;
            Ex=-15;
            Ey=0;
        }
        pType=ch-'0';
        //loads the position of the first person
        if(pType==3){
            Ex=Esx;
            Ey=Esy;
            Esx=-15;
            Esy=0;
            ph=th=0;
        }
    }
    //turns off axis
    else if(ch=='9'){
        if(mode==9){
            mode=0;
        }
        else{
            mode=9;
        }
    }
    else if(pType==3){
        //moves the metaphorical person forward
        if((ch=='w') || (ch=='W')){
            Ex+=dt*dx;
            Ey+=dt*dy; 
        }
        //moves the metaphorical person backward
        else if((ch=='s') || (ch=='S')){
            Ex-=dt*dx;
            Ey-=dt*dy;
        }
        //returns the metaphorical person to the central target
        else if((ch=='q') || (ch=='Q')){
            Ex=-15;
            Esx=-15;
            Ey=0;
            Esy=0;
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
        //scrolls through the test lighting objects
        else if(ch == 'p' || ch == 'P'){
            test+=1;
            if(test==4){
            	th=-90;
		        ph=90;
		        dim=42;
            }
            else if(test==5){
            	test=0;
            	th=0;
		        ph=0;
		        dim=6;
            }
        }
        else if(ch == 'o' || ch == 'O'){
        	if(test==4){
        		dim=6;
        		th=0;
        		ph=0;
        	}
            test-=1;
            if(test==-1){
            	test=4;
            	th=-90;
		        ph=90;
		        dim=42;
            }
        }
        //Taken from ex 21
        //moves the light along the y axis
        else if(ch == 'y'){
            Y+=0.1;
        }
        else if(ch == 'Y'){
            Y-=0.1;
        }
        //moves the light along the x axis
        else if(ch == 'x'){
            X+=0.1;
        }
        else if(ch == 'X'){
            X-=0.1;
        }
        //moves the light along the z axis
        else if(ch == 'z'){
            Z+=0.1;
        }
        else if(ch == 'Z'){
            Z-=0.1;
        }
    } 
    Project(fov,asp,dim);
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
        //turns the metaphorical person if the right arrow is pressed
        if (key == GLUT_KEY_RIGHT){
            th += 5;
        }
        //turns the metaphorical person if the left arrow is pressed
        else if(key == GLUT_KEY_LEFT){
            th -= 5;
        }
        //makes the metaphorical person look up when the up arrow is pressed
        else if(key == GLUT_KEY_UP){
            ph += 5;
        }
        //makes the metaphorical person look down when the down arrow is pressed
        else if(key == GLUT_KEY_DOWN){
            ph -= 5;
        }
    }
    
    //modulo angles with 360 to keep within our usage of degrees (not radians)
    th %= 360;
    ph %= 360;
    Project(fov,asp,dim);
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
    Project(fov,asp,dim); 
    ErrCheck("reshape");                    
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
    glutCreateWindow("Final Project: Luna McBride");
    //call display once loaded
    glutDisplayFunc(display);
    //call reshape when window is changed
    glutReshapeFunc(reshape);
    //calls the special function for special keys
    glutSpecialFunc(special);
    //calls the non-special function for non-special keys
    glutKeyboardFunc(keyboard);
    //call to the idle function
    glutIdleFunc(idle);
    //texture calls
    texture[0] = LoadTexBMP("grass.bmp");
    texture[1] = LoadTexBMP("gsky.bmp");
    texture[2] = LoadTexBMP("cob.bmp");
    texture[3] = LoadTexBMP("side.bmp");
    texture[4] = LoadTexBMP("rside.bmp");
    texture[5] = LoadTexBMP("roof.bmp");
    texture[6] = LoadTexBMP("wood.bmp");
    texture[7] = LoadTexBMP("conc.bmp");
    texture[8] = LoadTexBMP("rwhole.bmp");
    ErrCheck("main");
    //give GLUT the user interaction control
    glutMainLoop();
    //end
    return 0;
}