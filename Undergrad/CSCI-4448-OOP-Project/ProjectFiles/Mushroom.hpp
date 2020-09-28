#ifndef MUSHROOM_HPP
#define MUSHROOM_HPP
#include <string>
#include "shapes.hpp"

/**
 * The Mushroom Class.
 * This draws cute, little mushrooms
 * given a location, scale, rotation, and the texture array
 */
class Mushroom{
	public:
		//! Takes in the values to draw a mushroom
		Mushroom(double x, double y, double z, double scale, double rotation, unsigned int _texture[]){
			X=x; Y=y; Z=z;
			S=scale; R=rotation;
			texture=_texture;
			dome=new Dome();
			cyl=new Cyl;
			drawMushroom();
			ErrCheck("Mushroom");
		}
		~Mushroom(){
			
		}
		//! Draws a mushroom in smaller proportions
		void drawMushroom(){
			dome->draw(X,Y,Z+S/16,S/8,R,texture);
			cyl->draw(X,Y,Z,S/16.1,R,texture);
		}
	private:
		//! State variables for the mushroom
		double X;
		double Y;
		double Z;
		double R;
		double S;

		//! tools used to draw the mushroom
		Shapes* cyl;
		Shapes* dome;
		unsigned int *texture;
};

#endif