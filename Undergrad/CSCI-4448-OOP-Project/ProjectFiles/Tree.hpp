#ifndef TREE_HPP
#define TREE_HPP
#include <string>
#include "shapes.hpp"

/**
 * The Tree Class.
 * Draws Pine Trees given location, scale, and rotation
 */
class Tree{
	public:
		//! Sets the proper values then draws the tree
		Tree(double x, double y, double z, double scale, double rotation, unsigned int _texture[]){
			X=x; Y=y; Z=z;
			S=scale; R=rotation;
			texture=_texture;
			cyl=new Cyl();
			cone=new Cone;
			drawPineTree();
			ErrCheck("Tree");
		}
		~Tree(){
			
		}
		//! Just draws a tree while applying textures to the proper parts
		void drawPineTree(){
			//! draws the cone "branches" of the tree
			glColor3f(0.5,0.5,0.5);
			cone->draw(X,Y,Z+S/8,S,R,texture);

			//! Draws a tree base with a wood texture
			glEnable(GL_TEXTURE_2D);
		    glBindTexture(GL_TEXTURE_2D,texture[2]);
			cyl->draw(X,Y,Z,S/8.1,R,texture);
			glDisable(GL_TEXTURE_2D);
			glColor3f(1,1,1);
		}
	private:
		//! Placeholders for location, rotation, and scale
		double X;
		double Y;
		double Z;
		double R;
		double S;

		//! Cyl and Cone for tree as well as a texture array
		Shapes* cyl;
		Shapes* cone;
		unsigned int *texture;
};

#endif