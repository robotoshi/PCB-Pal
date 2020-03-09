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

	int step_x = 0;
	int step_y = 0;
	double phys_x = 0.0;
	double phys_y = 0.0;

	bool engaged = false;	// whether the pen is down or up
	Unit unit = MM;

	const double steps_per_in = 1000;	// TODO: calculate
	const double steps_per_mm = 300;	// TODO: calculate
	double steps_per_unit = steps_per_mm;

	void zero_here();

public:
	Printhead(const Stepper& xStepper, const Stepper& yStepper);

	void engage();
	void disengage();

	void set_move(double xdest, double ydest, double speed);
	bool tick();
	void disable();
	void enable();
	void cancel_move();
	void finish_move();

	Unit get_unit() { return unit; }
	void set_unit(Unit u) { unit = u; }
	bool is_engaged() { return engaged; }
};

#endif