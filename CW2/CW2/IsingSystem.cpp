#include "IsingSystem.h"

// colors
namespace colours {
	// note the f's here avoid warnings by telling C++ to read the numbers as floats and not doubles 
	GLfloat blue[] = { 0.1f, 0.3f, 0.9f, 1.0f };   // blue
	GLfloat red[] = { 1.0f, 0.2f, 0.1f, 0.2f };   // red
	GLfloat green[] = { 0.3f, 0.6f, 0.3f, 1.0f };   // green
}


// constructor
IsingSystem::IsingSystem(Window* set_win) {
	cout << "creating system, gridSize " << gridSize << endl;
	win = set_win;

	inverseTemperatureBeta = 1 / 4.0;
	repeatSetNum = 0;
	endNum = 500;
	endNumRun = 50;				//number of repeated readings for a single point of data
	n0 = 1000;				

	slowNotFast = 1;
	isActive = 0;

	// Allocate memory for the grid, remember to free the memory in destructor
	//   the point here is that each row of the grid is an array
	//   the grid itself is a an array of pointers, one for each row
	// Here we allocate the array of pointers
	grid = new int* [gridSize];
	// Now allocate the indidual rows
	for (int i = 0; i < gridSize; i++) {
		grid[i] = new int[gridSize];
	}

	// this sets the temperatre and initialises the spins grid
	Reset();

	
}

void IsingSystem::Reset() {

	//double initialTemp = 4.0;

	//setTemperature(initialTemp);

	// set the grid to -1
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			// position is (i,j)
			int pos[2] = { i,j };
			// set this spin to state -1
			setGrid(pos, -1);
		}
	}
}


// destructor
IsingSystem::~IsingSystem() {
	// Close the file (if open)
	if (logfile.is_open())
		logfile.close();

	// Delete the window
	delete win;

	// Delete the grid
	// First we delete the individual rows
	for (int i = 0; i < gridSize; i++)
		delete[] grid[i];
	// Finally delete the array of pointers
	delete[] grid;
}

// this draws the system
void IsingSystem::DrawSquares() {

	double drawScale = 2.0 / (gridSize * 1.1);

	// draw the particles
	double halfSize = 0.5;
	int halfGrid = gridSize / 2;
	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {

			double vec[2];
			vec[0] = x - halfGrid;
			vec[1] = y - halfGrid;

			// openGL magic
			glPushMatrix();
			// choose a color
			if (grid[x][y] == -1)
				glColor4fv(colours::green);
			else
				glColor4fv(colours::blue);
			// draw a rectangle for the particle
			glRectd(drawScale * (vec[0] - halfSize),
				drawScale * (vec[1] - halfSize),
				drawScale * (vec[0] + halfSize),
				drawScale * (vec[1] + halfSize));
			// openGL magic
			glPopMatrix();
		}
	}

	// print some information (at top left)
	// this ostringstream is a way to make a string with numbers and words (similar to cout << ... )
	ostringstream str;
	str << "beta " << inverseTemperatureBeta << " size " << gridSize;
	win->displayString(str, -0.9, 0.94, colours::red);

}


//function for collecting data over repeated runs
void IsingSystem::dataCollect(int exerise) {

	//setRepeatSetNum();
	pauseRunning();
	Reset();

	if (exerise == 1) {		//collects data for exercise one
		
		double T = 10;	//the amount of different TempBeta from 0.2 to o.7

		// this opens a logfile .csv named 
		logfile.open("Exercise_1.csv");	//opfile
		logfile << "Sweep,Mag,E_N,Beta,Temp" << endl;

		setFast();
		setRunning();
		for (int a = 0; a <= T; a++) {		//repeats everything over differnt temps beta

			inverseTemperatureBeta = 0.2 + (0.5 * (a / T));
			double Temp = 1 / inverseTemperatureBeta;

			for (int i = 0; i < endNumRun; i++) {	//repeats run endNumRun number of times

				for (int j = 0; j <= endNum; j++) {	//repeats sweeps endNum number of times

					logfile << j << "," << magnetisation() << "," << dimEnergyN() << "," << inverseTemperatureBeta << "," << Temp << endl;
					Update();
				}
				Reset();
			}
		}

		
	}
	else if (exerise == 2) {

		double T = 50;	//the amount of different TempBeta from 0.2 to o.7
		

		// this opens a logfile .csv named 
		logfile.open("Exercise_2.csv");	//opfile
		logfile << "Temp,Mag_mean,Mag_sd,abs_Mag_mean,abs_Mag_sd,Mag_exact,E_N_mean,E_N_sd" << endl;

		setFast();
		setRunning();

		for (int a = 0; a <= T; a++) {		//repeats everything over differnt temps 

			setTemperature(1.0 + (3 * (a / T)));

			double magSum = 0.0;
			double absMagSum = 0.0;
			double energyNSum = 0.0;
			double magSumSqu = 0.0;
			double absMagSumSqu = 0.0;
			double energyNSumSqu = 0.0;

			int m = 0;

			for (int i = 0; i <= n0; i++) {	//repeats sweeps n0  number of times to get to steady state
				Update();
			}

			for (m; m <= endNumRun; m++) {	//amount of times the properties will be measured in a steady state
				Update();
				
				double mag = 0.0;
				mag = magnetisation();

				magSum += mag;
				absMagSum += fabs(mag);
				energyNSum += dimEnergyN();
				magSumSqu += mag * mag;
				absMagSumSqu += fabs(mag) * fabs(mag);
				energyNSumSqu += dimEnergyN() * dimEnergyN();
			}

			double meanMag = magSum / m;
			double meanAbsMag = absMagSum / m;
			double meanEnergyN = energyNSum / m;

			double sdMag = sqrt((magSumSqu/m)-(meanMag*meanMag))/sqrt(m-1);
			double sdAbsMag = sqrt((absMagSumSqu / m) - (meanAbsMag * meanAbsMag))/ sqrt(m - 1);
			double sdEnergyN = sqrt((energyNSumSqu / m) - (meanEnergyN * meanEnergyN))/ sqrt(m - 1);

			logfile << (1/inverseTemperatureBeta) << "," << meanMag << "," << sdMag << "," << meanAbsMag << "," << sdAbsMag << "," << ExactIsingMagnetisation(inverseTemperatureBeta) << "," << meanEnergyN << "," << sdEnergyN << endl;

			Reset();
		}

	}
	else if (exerise == 3) {

		double T = 500;	//the amount of different TempBeta from 0.2 to CritialTempBeta

		// this opens a logfile .csv named 
		logfile.open("Exercise_3_.csv");	//opfile
		logfile << "Temp,T/Tc,Mag_exact,Mag_mf" << endl;

		setFast();
		setRunning();

		for (int a = 0; a <= T; a++) {		//repeats everything over differnt temps 

			double mag = 0.0;

			setTemperature((1/CritialTempBeta) * (0.003 + (a / T)));
			mag = ExactIsingMagnetisation(inverseTemperatureBeta);

			logfile << (1 / inverseTemperatureBeta) << "," << (CritialTempBeta/ inverseTemperatureBeta) << "," << mag  << endl;
		}

	}
	else if (exerise == 4) {
		double T =100;	//the amount of different TempBeta from 0.2 to o.7
		int n0 = 1000;    //number of sweeps to reach steady state

		// this opens a logfile .csv named 
		logfile.open("Exercise_4.csv");	//opfile
		logfile << "Temp,T/Tc,c,chi" << endl;

		setFast();
		setRunning();

		for (int a = 0; a < T; a++) {		//repeats everything over differnt temps 

			setTemperature((1 / CritialTempBeta)* (0.001 +  (a / T)));

			double magSum = 0.0;
			double energySum = 0.0;
			double magSumSqu = 0.0;
			double energySumSqu = 0.0;

			int m = 0;	//amount of times the properties will be measured in a steady state

			for (int i = 0; i <= n0; i++) {	//repeats sweeps n0  number of times to get to steady state
				Update();
			}

			for (m; m <= endNumRun; m++) {	//amount of times the properties will be measured in a steady state
				Update();

				magSum += magnetisation();
				energySum += dimEnergy();
				magSumSqu += magnetisation() * magnetisation();
				energySumSqu += dimEnergy() * dimEnergy();
			}

			double meanMag = magSum / m;		//mean mag
			double meanEnergyN = energySum / m;	//mean of E/J

			double varMag = (magSumSqu / m) - (meanMag * meanMag);		//variance of mag
			double varE = (energySumSqu / m) - (meanEnergyN * meanEnergyN);	//variance of E/J



			logfile << (1 / inverseTemperatureBeta) << "," << (CritialTempBeta / inverseTemperatureBeta) << "," << heatCapacity( varE) << "," << magneticSuscep(varMag) <<  endl;

			Reset();
		}

	}
	else if (exerise == 5) {

		int MaxXY = 10;	//max latrial distance for a pair
		double T = 5;	//the amount of different TempBeta
		// this opens a logfile .csv named 
		logfile.open("Exercise_5.csv");	//opfile
		logfile << "Temp,T/TC,x,y,D,G" << endl;

		setFast();
		setRunning();

		
		for (int a = 0; a < T; a++) {
			setInverseTemperature((1 / CritialTempBeta)* (0.001 + (a / T)));
			for (int x = 1; x <= MaxXY; x++) {
				for (int y = 0; y <= x; y++) {
					logfile << (1 / inverseTemperatureBeta) << "," << (CritialTempBeta / inverseTemperatureBeta) << "," << x << "," << y << "," << sqrt((x * x) + (y * y)) << "," << Gcorrelation(x, y) << endl;
				}
			}
		}
	}

	logfile.close();
	pauseRunning();
}

// attempt N spin flips, where N is the number of spins
void IsingSystem::MCsweep() {
	for (int i = 0; i < gridSize * gridSize; i++)
		attemptSpinFlip();
}

// here we attempt to flip a spin and accept/reject with Metropolis rule
void IsingSystem::attemptSpinFlip() {
	int pos[2];

	// random site
	pos[0] = rgen.randomInt(gridSize);
	pos[1] = rgen.randomInt(gridSize);

	double hloc = computeLocalField(pos);		//local mag field due to n.n.

	double dE = 2.0 * hloc * readGrid(pos);		//delta Energy
	if (dE < 0)
		flipSpin(pos);
	else if (rgen.random01() < exp(-dE))
		flipSpin(pos);

}

// NOTE: this returns the local field *divided by the temperature* (dimensionless quantity)
double IsingSystem::computeLocalField(int pos[]) {
	double result = 0.0;
	for (int i = 0; i < 4; i++) {
		int nborPos[2];
		setPosNeighbour(nborPos, pos, i, 1,0,1);
		result += readGrid(nborPos);
	}
	result *= inverseTemperatureBeta;
	return result;
}

// set the value of a grid cell for a particular position
void IsingSystem::setGrid(int pos[], int val) {
	grid[pos[0]][pos[1]] = val;
}

// read the grid cell for a given position
int IsingSystem::readGrid(int pos[]) {
	return grid[pos[0]][pos[1]];
}

// read the grid cell for a given position
void IsingSystem::flipSpin(int pos[]) {
	grid[pos[0]][pos[1]] = -grid[pos[0]][pos[1]];
}

//calculates magnetisation of the whole grid
double IsingSystem::magnetisation(){
	double mag = 0.0; {};
	int pos[2]{};
	for (int i = 0; i < gridSize; i++) {	//row of grid
		for (int j = 0; j < gridSize; j++) {	//coloum of grid
			pos[0] = i;
			pos[1] = j;				
			mag += grid[pos[0]][pos[1]];
		}
	}
	return mag/(gridSize* gridSize);
}

//calculates the modulus magnetisation using equation 16 from cw, tempBeta needs to more than critial temp Beta
double IsingSystem::ExactIsingMagnetisation(double tempBeta){

	double m_ = 0.0;

	if (tempBeta >= CritialTempBeta) {	//the condition to use this equation
		m_ = pow((1 - (1 / pow((sinh((tempBeta / CritialTempBeta) * log(1.0 + sqrt(2.0)))), 4.0))), 0.125);
	}

	return m_;
}

//calculates dimensionless energy per spin of the whole grid
double IsingSystem::dimEnergyN() {
	double E = 0.0; {};
	int pos[2]{};
	for (int i = 0; i < gridSize; i++) {	//row of grid
		for (int j = 0; j < gridSize; j++) {	//coloum of grid
			pos[0] = i;
			pos[1] = j;

			double spinE = grid[pos[0]][pos[1]];	//energy of spin choosen

			double nnE = 0.0;		//n.n energy
			for (int k = 0; k < 4; k++) {
				int nborPos[2];
				setPosNeighbour(nborPos, pos, k, 1, 0, 1);
				nnE += readGrid(nborPos);
			}
			E += (nnE * spinE);

		}
	}
	return E/(-1* gridSize* gridSize);  // factor of 0.5 is to account for double counting
}

//calculates dimensionless energy of the whole grid
double IsingSystem::dimEnergy() {
	double E = 0.0; {};
	int pos[2]{};
	for (int i = 0; i < gridSize; i++) {	//row of grid
		for (int j = 0; j < gridSize; j++) {	//coloum of grid
			pos[0] = i;
			pos[1] = j;

			double spinE = grid[pos[0]][pos[1]];	//energy of spin choosen

			double nnE = 0.0;		//n.n energy
			for (int k = 0; k < 4; k++) {
				int nborPos[2];
				setPosNeighbour(nborPos, pos, k, 1, 0, 1);
				nnE += readGrid(nborPos);
			}
			E += (nnE * spinE);

		}
	}
	return E / -1;  // factor of 0.5 is to account for double counting
}

//calculates dimensionless heat Capacity of system at temp T with the variance of E/J
double IsingSystem::heatCapacity(double varE){
	return (pow(inverseTemperatureBeta,2)/ pow(gridSize,2))*varE;
}

//calculates dimensionless magnetic susceptibility of system at temp T with the variance of M
double IsingSystem::magneticSuscep(double varMag){
	return (inverseTemperatureBeta * pow(gridSize, 2) * varMag);
}

//calculates spin-spin correlation function with a givien x and y length for whole grid
double IsingSystem::Gcorrelation(int x, int y) {		//note this one averges over many confs before changing pair
	int pos[2]{};
	double GsumGrid = 0.0;	//final grid sum of G
	double Npair = 0.0;		//counts pairs

	int maxA = 0;
	if (x == y || y == 0) {
		maxA = 4;
	}
	else if (x != y) {
		maxA = 8;
	}

	for (int i = 0; i <= n0; i++) {	//repeats sweeps n0  number of times to get to steady state
		Update();
	}

	for (int i = 0; i < gridSize; i++) {	//row of grid		this repeats for every point in the grid
		for (int j = 0; j < gridSize; j++) {	//coloum of grid
			pos[0] = i;						//position of first point in pair
			pos[1] = j;
			double spinE = readGrid(pos);	//spin of first point 

			for (int a = 0; a < maxA; a++) {	//chooses a neighbour x distance away in one latial direction and y the other direction
				int nborPos[2];			//neighbour position
				setPosNeighbour(nborPos, pos, a, x, y, 2);
				double GsumPair = 0.0;			//G sum for a pair
				Npair += 1.0;

				
				for (int m = 0; m <= endNumRun; m++) {	//amount of times G will be measured in a steady state
					Update();
					GsumPair += (spinE*readGrid(nborPos));
				}
				GsumGrid += (GsumPair / endNumRun);
				Reset();
			}
		}
	}
	return (GsumGrid / (2 * Npair));

};

// send back the position of a neighbour of a given grid cell at a laterial distance of step
// NOTE: we take care of periodic boundary conditions, also positions are integers now not doubles
void IsingSystem::setPosNeighbour(int setpos[], int pos[], int val, int x, int y, int  order) {
	if (order == 1) {
		switch (val) {	//val 0->3
		case 0:				//right
			setpos[0] = (pos[0] + x) % gridSize;
			setpos[1] = pos[1];
			break;
		case 1:				//left
			setpos[0] = (pos[0] - x + gridSize) % gridSize;
			setpos[1] = pos[1];
			break;
		case 2:				//up
			setpos[0] = pos[0];
			setpos[1] = (pos[1] + x) % gridSize;
			break;
		case 3:				//down
			setpos[0] = pos[0];
			setpos[1] = (pos[1] - x + gridSize) % gridSize;
			break;
		}
	}
	else if (order == 2) {	//val 0->7
		switch (val) {
		case 0:				//right up
			setpos[0] = (pos[0] + x) % gridSize;
			setpos[1] = (pos[1] + y) % gridSize;
			break;
		case 1:				//down right
			setpos[0] = (pos[0] + y) % gridSize;
			setpos[1] = (pos[1] - x + gridSize) % gridSize;
			break;
		case 2:				//left down
			setpos[0] = (pos[0] - x + gridSize) % gridSize;
			setpos[1] = (pos[1] - y + gridSize) % gridSize;
			break;
		case 3:				//up left
			setpos[0] = (pos[0] - y + gridSize) % gridSize;
			setpos[1] = (pos[1] + x) % gridSize;
			break;
		case 4:				//right down
			setpos[0] = (pos[0] + x) % gridSize;
			setpos[1] = (pos[1] - y + gridSize) % gridSize;
			break;
		case 5:				//down left
			setpos[0] = (pos[0] - y + gridSize) % gridSize;
			setpos[1] = (pos[1] - x + gridSize) % gridSize;
			break;
		case 6:				//left up
			setpos[0] = (pos[0] - x + gridSize) % gridSize;
			setpos[1] = (pos[1] + y) % gridSize;
			break;
		case 7:				//up right
			setpos[0] = (pos[0] + y) % gridSize;
			setpos[1] = (pos[1] + x) % gridSize;
			break;

		}
	}
}

// this is the update function which at the moment just does one mc sweep
void IsingSystem::Update() {
	MCsweep();
}

