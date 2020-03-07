#include "Arduino.h"
#include "stepper.h"

/*
 * Step   C0 C1 C2 C3
 *    0    1  0  1  0
 *    1    0  1  1  0
 *    2    0  1  0  1
 *    3    1  0  0  1
*/

// step_delay = 60L * 1000L * 1000L / number_of_steps / whatSpeed;

Stepper::Stepper( int steps, int pin1, int pin2, int pin3, int pin4 )
                : total_steps(steps), pin1(pin1), pin2(pin2), pin3(pin3), pin4(pin4), 
                  steps_to_go(0), current_position(0), step_delay(0), last_step_time(0) {

	// setup the pins on the microcontroller:
	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);
	pinMode(pin3, OUTPUT);
	pinMode(pin4, OUTPUT);
}

void Stepper::step_forward() {
	current_position = (current_position + 1) % 4;
	set_pins(current_position);
}

void Stepper::step_back() {
	current_position = (current_position == 0) ? 4 : current_position - 1;
	set_pins(current_position);
}

/**
	Sets all pins LOW so the motor can spin freely
*/
void Stepper::disable() {
	digitalWrite(pin1, LOW);
	digitalWrite(pin2, LOW);
	digitalWrite(pin3, LOW);
	digitalWrite(pin4, LOW);
}

void Stepper::enable() {}

void Stepper::set_pins(int position) {
	switch (position) {
		case 0:  // 1010
			digitalWrite(pin1, HIGH);
			digitalWrite(pin2, LOW);
			digitalWrite(pin3, HIGH);
			digitalWrite(pin4, LOW);
			break;
		case 1:  // 0110
			digitalWrite(pin1, LOW);
			digitalWrite(pin2, HIGH);
			digitalWrite(pin3, HIGH);
			digitalWrite(pin4, LOW);
			break;
		case 2:  // 0101
			digitalWrite(pin1, LOW);
			digitalWrite(pin2, HIGH);
			digitalWrite(pin3, LOW);
			digitalWrite(pin4, HIGH);
			break;
		case 3:  // 1001
			digitalWrite(pin1, HIGH);
			digitalWrite(pin2, LOW);
			digitalWrite(pin3, LOW);
			digitalWrite(pin4, HIGH);
			break;
		default:  // 0000
			disable();
	}
}

/////////////////////////////////////////////////////////////////////////////

/**
	Set the parameters for a move.
	steps: number of steps to take
	time: delay between the moves in us
*/
int Stepper::set_move(int steps, unsigned long s_delay) {
	step_delay = s_delay;
	if (steps_to_go == 0) {
		steps_to_go = steps;
		return steps_to_go;
	} else return 0;

}

void Stepper::cancel_move() {
	steps_to_go = 0;
}

/**
	Call this as often as you want to advance the stepper (if it's time)
*/
int Stepper::tick() {
	if (steps_to_go != 0) {
		unsigned long now = micros();
		if (now >= last_step_time + step_delay) {
			last_step_time = now;
			advance();
			Serial.println(now - last_step_time);
		}
	}
	return steps_to_go;
}

/**
	Advance the stepper one step in the direction of the current move
*/
int Stepper::advance() {
	if (steps_to_go > 0) {
		step_forward();
		--steps_to_go;
	} else if (steps_to_go < 0) {
		step_back();
		++steps_to_go;
	}
	return steps_to_go;
}

/**
	Block to finish the current move immediately
*/
void Stepper::finish_move() {
	while (tick() != 0);
}