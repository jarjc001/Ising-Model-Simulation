#pragma once

#include <gl/freeglut.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <string>
#include <sstream>

#include "Window.h"
#include "rnd.h"

using namespace std;

// IsingSystem class
// this is where the Ising simulation happens
class IsingSystem {
private:
	Window* win;  // pointer to the window in which the system is running
				  // (this is not actually used but we keep it, just in case)

				  // size of grid: "static const" means that this number is a fixed constant
	static const int gridSize = 40;
	int** grid;  // this will be a 2d array that stores whether each site is occupied

				 // random number generator, class name is rnd, instance is rgen
	rnd rgen;

	// this variable is 1 if the system is running, 0 for paused
	int isActive;

	// this variable is 1 if we are in "slow" mode, 0 in fast mode
	int slowNotFast;

	// is sim resetting and repeating (1) or stopping (0) when endNum is reached (for data collection)
	int repeatSetNum;

	// number of sweeps at which the simulation will stop, reset and repeat
	int endNum;

	//Max number of runs before stopping auto repeat sim
	int endNumRun;

	//number of sweeps to reach steady state
	int n0;

	// this is the inverse temperature parameter beta
	double inverseTemperatureBeta;

	//critial temp beta
	double CritialTempBeta = log(1.0 + sqrt(2.0)) / 2.0;

	// this an output file (in case we need one)
	ofstream logfile;

public:
	// constructor
	IsingSystem(Window* set_win);
	// destructor
	~IsingSystem();

	// this sets the seed for the random numbers
	void setSeed(int s) { rgen.setSeed(s); }

	// these functions are used to update the system
	double computeLocalField(int pos[]);
	void attemptSpinFlip();
	void MCsweep();

	// increase or decrease the temperature (arbitrary increments)
	void Hotter() { inverseTemperatureBeta -= 0.05; }
	void Colder() { inverseTemperatureBeta += 0.05; }

	void setTemperature(double TT) { inverseTemperatureBeta = 1.0 / TT; }
	void setInverseTemperature(double TT) { inverseTemperatureBeta = TT; }

	// set and read grid entries associated with a given position
	// NOTE! the grid is not accessed directly, only via these functions
	void setGrid(int pos[], int val);
	int readGrid(int pos[]);
	void flipSpin(int pos[]);

	//calculates magnetisation of the whole grid
	double magnetisation();

	//calculates modulus magnetisation using equation 16 from cw, temp needs to less than critial temp
	double ExactIsingMagnetisation(double temp);

	//calculates dimensionless energy per spin of the whole grid
	double dimEnergyN();

	//calculates dimensionless energy of the whole grid
	double dimEnergy();

	//calculates dimensionless heat Capacity of system at temp T with the variance of E/J
	double heatCapacity(double varE);

	//calculates dimensionless magnetic susceptibility of system at temp T with the variance of M
	double magneticSuscep(double varMag);

	//calculates spin-spin correlation function with a givien x and y length for whole grid
	double Gcorrelation(int x, int y);

	// reset temperature to default value and re-initialise the grid
	void Reset();

	// "user interface" for the isActive variable
	int  isRunning() { return isActive; }
	void setRunning() { isActive = 1; }
	void pauseRunning() { isActive = 0; }

	// "user interface" for the slowNotFast variable
	int isSlow() { return slowNotFast; }
	void setSlow() { slowNotFast = 1; }
	void setFast() { slowNotFast = 0; }

	// set whether it repeat runs or not for data collection
	void setRepeatSetNum() { repeatSetNum = 1; }
	void stopRepeatSetNum() { repeatSetNum = 0; }

	// set setpos to the position of a neighbour of pos
	// the neighbour to use is determined by val (=0,1,2,3)
	// step is how many far the neighbour is and the order is whether it is one direction or two diretcion
	// beware: behavior is not defined for val>=4 or val<0
	void setPosNeighbour(int setpos[], int pos[], int val, int x, int y, int order);

	// update the system: makes a Monte Carlo sweep
	void Update();

	// function for collecting data over repeated runs
	void dataCollect(int exerise);

	// draws the system as squares
	void DrawSquares();
};

