#include "CSCIx229.h"

//mostly taken from Example 9
int spot=0;
int local=1;
int pType=2;
int id=1;
int mode=9;
int test=0;
int th=0;
int ph=0;
int side=1;
int fov=55;
float sco=20;    
float Exp=20;
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
double rep=5;
double reps;
int num=90;
unsigned int texture[7];
//taken from example 15
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
int ch        =  90;
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

//taken from ex15
void idle()
{
	if(id==1){
        //  Elapsed time in seconds
	   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	   zh = fmod(90*t,360.0);
	   //  Tell GLUT it is necessary to redisplay the scene
	   glutPostRedisplay();
	}
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
    //glTexCoord2f((t/360.0),(p/180.0+0.5));
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
        	glNormal3f(Sin(t)*Cos(p), Sin(p), Cos(t)*Cos(p));
            Vertex(t,p);
            Vertex(t,p+d);
        }
        glEnd();
    }

    //load save state
    glPopMatrix();   
}
//Using the Example 15 cube
static void sky(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th){
    int p=2;
    //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
    //glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[5]);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   //glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(p,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(p,p); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,p); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   //glNormal3f(0,0,-1);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(p,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(p,p); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,p); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glNormal3f(1,0,0);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(p,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(p,p); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,p); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glNormal3f(-1,0,0);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(p,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(p,p); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,p); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glNormal3f(0,1,0);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(p,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(p,p); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,p); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glNormal3f(0,-1,0);
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
}

//taken from Example 8, altered using sphere1 and Example 13
static void bush(double x, double y, double z, double r){
    const int d=5;
    int th,ph;

    //hold onto current transformation state
    glPushMatrix();
    //move and scale
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(90,1,0,0);

    //bands
    //glEnable(GL_TEXTURE_2D);
    glColor3f(0.418,0.555,0.137);
    //glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	//glBindTexture(GL_TEXTURE_2D,texture[1]);
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

    //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90);
   for (th=0;th<=360;th+=d)
   {
   	  glNormal3f(0,0,-1);
   	  glTexCoord2f(2*Cos(th)+0.5,2*Sin(th)+0.5);
      Vertex(th,d-90);
   }
   glEnd();

   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
      	 glNormal3f(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
         glTexCoord2f((th/360.0),(ph/180.0)+0.5);
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (th=0;th<=360;th+=d)
   {
   	  glNormal3f(0,0,1);
   	  glTexCoord2f(2*Cos(th)+0.5,2*Sin(th)+0.5);
      Vertex(th,90-d);
   }
   glEnd();
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    glDisable(GL_TEXTURE_2D);
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
    glRotated(-90,0,1,0);
    glScaled(0.5*r,2*r,0.3*r);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[4]);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    for(p=-90;p<90;p+=d){
        glBegin(GL_QUAD_STRIP);
        for(t=0;t<=360;t+=d){
        	glTexCoord2f(2/(t/360.0),(2/(p/180.0))+0.5);
        	glNormal3f(-(Sin(t)*Cos(p)), -Sin(p), -(Cos(t)*Cos(p)));
            Vertex(t,p);
            Vertex(t,p+d);
        }
        glEnd();
    }
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//Replaced old one with Example 14's for lighting purposes
static void circle(double x, double y, double z, double r, double ro){
    int k;
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(ro,1,0,0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,0.25);
	glTexCoord2f(0.5,0.5); glVertex3f(0,0,0);
	for (k=0;k<=360;k+=10)
	{
		glTexCoord2f(0.65/2*Cos(k)+0.5,0.7/2*Sin(k)+0.5);
		glVertex2f(Cos(k),Sin(k));
	}
	glEnd();
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    glPopMatrix();
}

static void can(double x, double y, double z,double r){
    const int d=5;
    int th,ph=0;
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glRotated(90,1,0,0);
    side=1;
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
    circle(0,0.45,0,0.893,90);
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    glBegin(GL_QUAD_STRIP);
    for (ph=d-90;ph<=90;ph+=d){
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d){
      	glTexCoord2f((th/180.0),((ph/720.0+0.5)));
      	glNormal3f(Sin(th), 0, Cos(th));
         Vertex(th,ph/4);
         Vertex(th,(ph/4)+d);
      }
      glEnd();
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    circle(0,-0.35,0,0.933,90);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
}

static void cone(double x, double y, double z, double r){
    int i,d=10;
    glColor3f(0.133,0.543,0.133);
    glPushMatrix();
    glRotated(90,1,0,0);
    glTranslated(x,y,z);
    glScaled(r,r,r);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture[6]);
    glBegin(GL_TRIANGLE_STRIP);
    for (i=0;i<=360;i+=d){
    	glTexCoord2f(6/2*Cos(i)+0.5,6/2*Sin(i)+0.5);
    	glNormal3f(-Sin(i)*Cos(0), -Sin(0), -(Cos(i)*Cos(0)));
        Vertex(i,0);
        glTexCoord2f(0,2);
        glVertex3d(0,2,0);
    }
    glNormal3f(0,-1,0); glVertex3d(0,2,0);
    glEnd();
    circle(0,0,0,1,90);
    glRotated(90,1,0,0);
    can(0,0,0,0.3);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}



static void tree(double x, double y, double z, double r){
    glPushMatrix();
    glScaled(r,r,r);
    glTranslated(x,y,z);
    can(0,0,0,1.5);
    can(0,0,1,1.4);
    can(0,0,2,1.3);
    can(0,0,2.8,1.2);
    can(0,0,3.5,1.1);
    can(0,0,4,1);
    can(0,0,4.3,0.9);
    can(0,0,4.6,0.8);
    can(0,0,4.9,0.7);
    can(0,0,5.2,0.6);
    can(0,0,5.5,0.5);
    can(0,0,5.7,0.4);
    can(0,0,5.9,0.3);
    can(0,0,6.1,0.2);
    can(0,0,6.2,0.1);
    leaf(-2,0,6,0,0,1,1);
    leaf(2,0,6,0,0,1,1);
    leaf(0,-2,6,0,0,0,1);
    leaf(0,2,6,0,0,0,1);
    glPopMatrix();
}

//Taken directly from Example 15 and Example 21(spotlight)
void light(){
	//  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {X+5*Cos(zh),Y+ylight,Z+5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
        sphere(Position[0],Position[1],Position[2] , 0.1);
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
      
      
}

void area(){
    if(pType!=3){
        glRotated(-90,1,0,0);
        light();
        if(test==0){
        	sky(0,0,0,50,50,50,0);
        }
        else if(test==1){
        	bush(0,0,0,1);
        }
        else if(test==2){
        	side=1;
        	circle(0,0,0,1,0);
        }
        else if(test==3){
        	ground(0,0,1,1,1,1,180);
        }
        else if(test==4){
        	can(0,0,0,1);
        }
        else if(test==5){
        	leaf(0,0,0,0,0,0,1);
        }
        else if(test==6){
        	cone(0,0,0,1);
        }
        else if(test==7){
        	tree(0,0,0,1);
        }
        glDisable(GL_LIGHTING);
    }
    else{
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
		//glColor3f(0.418,0.555,0.137);
		bush(2,0,4,0.5);
		bush(-2,-4,0,0.4);
		bush(4,-4,0,0.3);
		bush(8,-4,0,0.2);
		bush(1,-4,0,0.2);
		bush(12,14,0,0.5);
		bush(-12,-14,0,0.4);
		bush(14,-14,0,0.3);
		bush(18,-14,0,0.2);
		bush(11,-14,0,0.2);
		sky(0,0,-1,30,30,10,180);
		ground(0,0,-1,30,30,10,180);
		cone(6,6,0,1);
		cone(6,-6,0,1);
		cone(7,-3,0,0.5);
		cone(2,-9,0,0.4);
		cone(-12,9,0,0.3);
		cone(15,11,0,2);
		cone(-4,15,0,0.25);
		cone(-12,11,0,0.1);
		//Center Target
		glColor3f(0.625,0.32,0.176);
		glPushMatrix();
		glEnable(GL_POLYGON_OFFSET_FILL);
		glRotated(1,1,0,0);
		glPolygonOffset(2,2);
		circle(0,0,0,1.1,0);
		glColor3f(1,1,1);
		glPolygonOffset(1,1);
		circle(0,0,0,1,0);
		glPolygonOffset(0,0);
		glColor3f(0,0,0);
		circle(0,0,0,0.1,0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPopMatrix();
    }
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
    	id=1;
        double E1=-2*dim*Sin(th)*Cos(ph);
        double E2=+2*dim*Sin(ph);
        double E3=+2*dim*Cos(th)*Cos(ph);
        gluLookAt(E1,E2,E3,0,0,0,0,Cos(ph),0);
    }
    else if(pType==3){
    	float Direction[] = {Cos(th)*Sin(ph),Sin(th)*Sin(ph),-Cos(ph),0};
      	id=0;
      	float Position[]  = {Ex,Ey,Ez,-50};
      	sphere(Position[0],Position[1],Position[2],0.1);
      	 //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
       //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
      	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
        glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
        glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);
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
        Print("Angle: %d,%d   Dim: %.1f   Fov: %d ", th, ph, dim,fov);
        if(pType == 2){
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
    else if('2'<=ch && ch<='3'){
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
    else if(ch=='t'){
    	if(spot==1){
    		spot=0;
    	}
    	else{
    		spot=1;
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
        else if(ch == 'i'){
            if(id==1){
            	id=0;
            }
            else{
            	X=0;
            	Y=0;
            	Z=0;
            	id=1;
            }
        }
        else if(ch == 'p'){
            test+=1;
            if(test==8){
            	test=0;
            }
        }
        else if(ch == 'o'){
            test-=1;
            if(test==-1){
            	test=7;
            }
        }
        //Taken from ex 21
        else if(ch == 'y'){
            Y+=0.1;
        }
        else if(ch == 'Y'){
            Y-=0.1;
        }
        else if(ch == 'x'){
            X+=0.1;
        }
        else if(ch == 'X'){
            X-=0.1;
        }
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
    glutCreateWindow("Assignment 3: Luna McBride");
    //call display once loaded
    glutDisplayFunc(display);
    //call reshape when window is changed
    glutReshapeFunc(reshape);
    //calls the special function for special keys
    glutSpecialFunc(special);
    //calls the non-special function for non-special keys
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    texture[0] = LoadTexBMP("grass.bmp");
    texture[1] = LoadTexBMP("redbush.bmp");
    texture[2] = LoadTexBMP("bark.bmp");
    texture[3] = LoadTexBMP("rings.bmp");
    texture[4] = LoadTexBMP("leaf2.bmp");
    texture[5] = LoadTexBMP("night2.bmp");
    texture[6] = LoadTexBMP("pine.bmp");
    //give GLUT the user interaction control
    glutMainLoop();
    //end
    return 0;
}