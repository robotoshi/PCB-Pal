#include "Arduino.h"
#include "printhead.h"
#include "stepper.h"

Printhead::Printhead( int xsteps, int xpin1, int xpin2, int xpin3, int xpin4, 
                      int ysteps, int ypin1, int ypin2, int ypin3, int ypin4 )
                    : xStepper(xsteps, xpin1, xpin2, xpin3, xpin4), 
                      yStepper(ysteps, ypin1, ypin2, ypin3, ypin4), 
                      step_x(0), step_y(0), phys_x(0), phys_y(0), 
                      engaged(false), unit(MM) {}


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