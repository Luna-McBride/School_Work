#ifndef FLYWEIGHT_HPP
#define FLYWEIGHT_HPP
#include <string>
#include <stdlib.h>
#include "Tree.hpp"
#include "Mushroom.hpp"
#include <cstdlib>

/**
 * The Flyweight Class.
 * The powerhouse function that does the intensive
 * calculations to draw the objects semi-randomly
 *
 */

class Flyweight{
	public:
		//! sets the texture and a test variable
		Flyweight(unsigned int *_texture){
			test=test+1;
			texture=_texture;
		}
		//! Randomizes the position and sizes into arrays
		void setFlyweight(){
			int valueNegator;
			for(int k=0;k<pineCount;k++){
				//! Randomly picks if the value will be positive or negative
				valueNegator=randomNegation();
				//! Makes negator positive for scale-based values
				if(k%3==2){
					valueNegator=1;
				}
				dataPlaces[k]=getRandomDouble(valueNegator);
				mushPlaces[k]=getRandomDouble(valueNegator);
			}
			ErrCheck("setFlyweight");
		}
		void drawItems(){
			//! Only sets the values the first time through
			if(test==1){
				setFlyweight();
			}
			//! Draws the items with locations and scales being from the randomized array
			for(int i=0;i<pineTreeNumber;i++){
					int i3=3*i;
					double scale=dataPlaces[2+i3]/40;
					new Tree(dataPlaces[i3],dataPlaces[1+i3],0,scale+3,0,texture);
					new Mushroom(mushPlaces[i3], mushPlaces[1+i3], 0, scale,0, texture);
			}
			ErrCheck("drawTrees");
		}
	private:
		//! Variables to set how many items to draw
		int pineTreeNumber=200;
		int pineCount=pineTreeNumber*3;

		//! Random Number Generator
		double getRandomDouble(int k){
			int c=rand()%150;
			return k*(c/5)+k*3;
		}

		//! Chooser of which values will be negative and which will be positive
		double randomNegation(){
			int c=rand()%20;
			if(c<10){
				return -1;
			}
			return 1;
		}
		//! Test variable to make the numbers not continuously randomize
		static int test;

		//! Arrays for tree and mushroom placement values
		static double dataPlaces[600];
		static double mushPlaces[600];
	protected:
		unsigned int *texture;
};

//! Initial assignment of array values and the test variable
int Flyweight::test=0;
double Flyweight::dataPlaces[]={0};
double Flyweight::mushPlaces[]={0};

#endif