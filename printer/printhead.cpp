#include "Arduino.h"
#include "printhead.h"
#include "stepper.h"

Printhead::Printhead( const Stepper& xStepper, const Stepper& yStepper ) :
                      xStepper(Stepper(xStepper)), yStepper(Stepper(yStepper)) {}

void Printhead::set_dest(double xdest, double ydest, double speed) {
	// TODO: math
}

bool Printhead::tick() {
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
	engaged = true;
}

void Printhead::disengage() {
	engaged = false;
}


void Printhead::zero_here() {
	step_x = 0;
	step_y = 0;
	phys_x = 0;
	phys_y = 0;
}