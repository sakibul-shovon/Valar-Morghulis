

#include "screen.h"  
#include <iostream>
using namespace std;



void sayHello(){
    cout<<"Running";
}


void playButton(int mx, int my, bool &home_menu, bool &start_screen) {
	if ((mx >= 113 && mx <= 268) && (my >= 524 && my <= 573)) {
		home_menu = 0;
		start_screen = 1;
	}
}


void aboutUs(bool &home_menu, bool &about_us){
	home_menu = 0;
	about_us = 1;
	
}

