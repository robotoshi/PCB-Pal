#ifndef H_GCODES
#define H_GCODES

#include "Arduino.h"
#include "gcommand.h"

#define INVOKE(p, m) ( ((p)->*(m))() )
#define METHOD_DEF() typedef bool (GInterpreter::*Method)()

class GInterpreter {
public:
	bool interpret(char* line);
	bool execute();
	GInterpreter() {}

private:
	METHOD_DEF(); // This one is for the function decls below to use
	GCommand command;
	Method method; // can't use the dang typedef yet :/

	// because the gcode functions are stateful in general, they need to know when to reset their state, 
	// namely if they're been interrupted by an urgent command
	bool first_time = true;

	Method get_method();
	bool done();	// all gcode procedures should call this to return control when finished
	
	bool test();
	bool echo();

	// Actual codes lie below...
	// See the file that outlines functionality in more detail

	bool G0();		// Move quickly
	bool G1();		// Move and draw

	bool M73();	// set progress
	bool M115();	// query firmware
	bool G90();	// use absolute coordinates
	bool G91();	// use relative coordinates
	bool M83();	// extruder relative mode

	bool G92();	// set position of axes

	bool G20();	// set units to inches
	bool G21();	// set units to millimeters

	bool M3();		// spindle on
	bool M5();		// spindle off

	bool G53();	// move in machine coords
	bool G54();	// to G59, set workspace coords or change to workspace

	bool M17();	// steppers on
	bool M18();	// steppers off

	bool M203();	// set max feedrate
	bool M92();	// set steps per unit
	bool G94();	// ^

	bool G27();	// park toolhead
	bool G28();	// auto-home

	bool M0();		// stop
	bool M112();	// emergency stop!
	bool M410();	// stop steppers now
	bool M2();		// end of program
	bool M108();	// resume

	bool M400();	// wait for moves to finish
};

METHOD_DEF(); // this one is for the defs to use
#endif