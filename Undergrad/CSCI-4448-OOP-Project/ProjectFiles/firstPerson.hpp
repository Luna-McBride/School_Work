#ifndef FIRSTPERSON_HPP
#define FIRSTPERSON_HPP

/**
 * The First Person Class.
 * The camera figure and "player" of this simulation.
 */

class FirstPerson{
	public:
		FirstPerson(){
			changeView();
		}
		//! Updates the view and looking position
		static void changeView(){
			gluLookAt(Ex,Ey,Ez,Ex+Dx,Ey+Dy,Ez+Dz,0,0,1);
		}
		//! reshapes the window if someone wants to resize it
		void reshapeWindow(int width, int height){
			//! calculates the aspect ratio then posts it to the screen
		    asp = (height>0) ? (double)width/height : 1;
		    glViewport(0,0,width,height);
		    Project(fov,asp,dim); 
		    ErrCheck("reshape");                    
		}
		//Tells the "player" how to move
		void keyboard(unsigned char ch){
			//! Closes the window
			if(ch==27){
				exit(0);
			}

			//! Does the calculations to move forward
	        else if((ch=='w') || (ch=='W')){
	            Ex+=Dt*Dx;
	            Ey+=Dt*Dy; 
	        }
	        
	        //!does the calculations to nove backward
	        else if((ch=='s') || (ch=='S')){
	            Ex-=Dt*Dx;
	            Ey-=Dt*Dy;
	        }

	        //! Redisplays the scene with the new values
	        changeView();
		    Project(fov,asp,dim);
		    glutPostRedisplay();
		}

		//! Uses the arrow keys to look around
		void specialKeyboard(int key){
				//! Makes the player turn right
		        if (key == GLUT_KEY_RIGHT){
		            th-=5;
		            Dx=Cos(th);
		            Dy=Sin(th);
		        }
		        //! Makes the player turn left
		        else if(key == GLUT_KEY_LEFT){
		            th+=5;
		            Dx=Cos(th);
		            Dy=Sin(th);
		        }
		        //! Makes the player look upward only until 40
		        else if(key == GLUT_KEY_UP){
		            if(ph<40){
		                ph += 5;
		                Dz=Sin(ph);
		            }
		        }
		        //! Makes the player look downward only until -5
		        else if(key == GLUT_KEY_DOWN){
		            if(ph>-5){
		                ph -= 5;
		                Dz=Sin(ph);
		            }
		        }


		    //! Keeps looking directions in a 360 degree plane
		    th %= 360;
		    ph %= 360;
		    changeView();
		    Project(fov,asp,dim);
		    glutPostRedisplay();
		}
	protected:
		//! Location variables. Define placement of the First Person
		static double Ex;
		static double Ey;
		static double Ez;
		
		//! Change variables. These are what change to cause movement
		static double Dx;
		static double Dy;
		static double Dz;
		static double Dt;

		//! Defines the fields for the projection
		int fov=55;
		double asp=1;
		double dim=6;

		//! Angle of vision
		int th=0;
		int ph=0;
};

//Defines the variables to set variables
double FirstPerson::Ex=0;
double FirstPerson::Ey=0;
double FirstPerson::Ez=1;
double FirstPerson::Dx=1;
double FirstPerson::Dy=0;
double FirstPerson::Dz=0;
double FirstPerson::Dt=0.1;

#endif