#include "Arduino.h"
#include "printhead.h"
#include "stepper.h"

Printhead::Printhead( const Stepper& xStepper, const Stepper& yStepper ) :
                      xStepper(Stepper(xStepper)), yStepper(Stepper(yStepper)) {}
/**
	Calculate the stepper delay and number of steps and set the move for each stepper. 
	speed is in units/s
*/
void Printhead::set_move(double xdest, double ydest, double speed) {
	double xdist = xdest - phys_x;	// physical distance
	double ydist = ydest - phys_y;

	double xstep_dist_f = xdist * steps_per_unit;	// distance in # of steps (unrounded intermediate val)
	double ystep_dist_f = ydist * steps_per_unit;
	
	int xstep_dist = round(xstep_dist_f);	// rounded distance in # of steps
	int ystep_dist = round(ystep_dist_f);

	// absolute physical distance / (physical distance per second) == full moves per us
	double step_rate = 1000000.0 * sqrt(sq(xdist) + sq(ydist)) / speed;
	Serial.println(step_rate);


	Serial.print(xstep_dist);
	if (xstep_dist != 0) {
		xStepper.set_move(xstep_dist, round(step_rate / xstep_dist_f));	// (full moves per second) / (# of steps) == us per step
		Serial.print(" @ ");
		Serial.print((step_rate / xstep_dist_f));
		Serial.print("us/step");
	}
	Serial.print(" | ");
	Serial.print(ystep_dist);
	if (ystep_dist != 0) {
		yStepper.set_move(ystep_dist, round(step_rate / ystep_dist_f));
		Serial.print(" @ ");
		Serial.print((step_rate / ystep_dist_f));
		Serial.println("us/step");
	}

	
	step_x += xstep_dist;
	step_y += ystep_dist;
	phys_x = xdest;
	phys_y = ydest;
}

bool Printhead::tick() {
	// if either steper still has steps to go (!= 0)
	return (xStepper.tick() || yStepper.tick());
}

void Printhead::disable() {
	xStepper.disable();
	yStepper.disable();
}

void Printhead::enable() {
	xStepper.enable();
	yStepper.enable();
}

void Printhead::cancel_move() {
	xStepper.cancel_move();
	yStepper.cancel_move();
}

void Printhead::finish_move() {
	xStepper.finish_move();
	yStepper.finish_move();
}

// TODO: Solonoid stuff
void Printhead::engage() {
	if (engaged) return;
	engaged = true;
}

void Printhead::disengage() {
	if (!engaged) return;
	engaged = false;
}


void Printhead::zero_here() {
	step_x = 0;
	step_y = 0;
	phys_x = 0;
	phys_y = 0;
}