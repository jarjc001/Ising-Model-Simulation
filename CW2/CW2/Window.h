#pragma once
#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Window {
public:
	string title;
	int size[2];
	int pos[2];

	int winID;

	void locateOnScreen() {
		// the fx sets where on the screen the window will appear
		// (values should be between 0 and 1)
		double fx[] = { 0.7,0.5 };
		pos[0] = (int)((glutGet(GLUT_SCREEN_WIDTH) - size[0]) * fx[0]);
		pos[1] = (int)((glutGet(GLUT_SCREEN_HEIGHT) - size[1]) * fx[1]);
	}

	// constructor, size is in pixels
	Window(int set_size[], string& set_title);
	~Window() { glutDestroyWindow(winID); }

	// function which prints a string to the screen, at a given position, with a given color
	// note position is "absolute", not easy to get two strings spaced one above each other like this
	void displayString(ostringstream& str, double x, double y, GLfloat col[]);

};




