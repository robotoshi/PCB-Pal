#ifndef STEPPER_H
#define STEPPER_H

#include "Arduino.h"

/**
	The built-in Arduino stepper library is trash and blocks, 
	so here we are with the new and improved stepper class
*/
class Stepper {
private:
	int pin1;
	int pin2;
	int pin3;
	int pin4;
	int total_steps;

	int steps_to_go = 0;
	int current_position = 0;		// which step the motor is on

	unsigned long step_delay = 1000; 	// delay between steps, in us, based on speed
	unsigned long last_step_time = 0;	// time stamp in us of when the last step was taken

	void set_pins(int position);
	void step_forward();
	void step_back();
	int advance();

public:
	Stepper(int steps, int pin1, int pin2, int pin3, int pin4);

	int set_move(int steps, unsigned long s_delay);
	int tick();
	void disable();
	void enable();
	void cancel_move();
	void finish_move();
};

#endif