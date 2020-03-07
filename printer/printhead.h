#ifndef PRINTHEAD_H
#define PRINTHEAD_H

#include "Arduino.h"
#include "stepper.h"

enum Unit {
	MM,
	MIL,
	IN
};

/**
	A model of the printhead itself, composed of two steppers
*/
class Printhead {
private:
	Stepper xStepper;
	Stepper yStepper;

	int step_x;
	int step_y;
	double phys_x;
	double phys_y;

	bool engaged;	// whether the pen is down or up
	Unit unit;

	const double steps_per_in = 100;	// TODO: calculate
	const double steps_per_mm = 500;	// TODO: calculate

	void zero_here();

public:
	Printhead( int xsteps, int xpin1, int xpin2, int xpin3, int xpin4, 
	           int ysteps, int ypin1, int ypin2, int ypin3, int ypin4 );

	void engage();
	void disengage();

	void set_dest(double xdest, double ydest, double speed);
	bool tick();
	void disable();
	void enable();
	void cancel_move();
	void finish_move();

	Unit get_unit() { return unit; }
	void set_unit(Unit u) { unit = u; }
};

#endif