#ifndef H_STEPPER
#define H_STEPPER

#include "Arduino.h"

/**
	The built-in Arduino stepper library is trash and blocks, 
	so here we are with the new and improved stepper class
*/
class Stepper {
private:
	int motor_pin_1;
	int motor_pin_2;
	int motor_pin_3;
	int motor_pin_4;
	int total_steps;

	int steps_to_go;
	int current_position;		// which step the motor is on

	unsigned long step_delay; // delay between steps, in us, based on speed
	unsigned long last_step_time; // time stamp in us of when the last step was taken

	void set_pins(int position);
	void step_forward();
	void step_back();

public:
	Stepper(int steps, int pin1, int pin2, int pin3, int pin4);

	int set_move(int steps, unsigned long step_delay);
	int tick();
	void disable();
	void cancel_move();
	void finish_move();
	int advance();
};

#endif