#ifndef OBSERVER_HPP
#define OBSERVER_HPP
#include "firstPerson.hpp"

/**
 * The Observer Class.
 * This is basically how most of the listeners work 
 *   with classes.
 */
class Observer{
	public:
		~Observer(){
			delete fp;
		}
		//! Notify functions to make the first person aware of changes
		static void notify(int key, int x, int y){
			fp->specialKeyboard(key);
		}
		static void notify(unsigned char ch, int x,int y){
			fp->keyboard(ch);
		}
		static void notify(int width, int height){
			fp->reshapeWindow(width, height);
		}
		static FirstPerson* fp;
};

//! Initializer for the First Person
FirstPerson* Observer::fp=new FirstPerson;

#endif