#ifndef H_GCODES
#define H_GCODES

#include "Arduino.h"
#include "gcommand.h"

typedef int (*Procedure)();

namespace gcodes {
	Procedure dispatch(GCommand command);

	Procedure test();
	Procedure echo();
	Procedure other();
	Procedure done();	// all gcode procedures should call this to return control when finished

	// Actual codes lie below...
	// See the file that outlines functionality in more detail

	Procedure G0();		// Move quickly
	Procedure G1();		// Move and draw

	Procedure M73();	// set progress
	Procedure M115();	// query firmware
	Procedure G90();	// use absolute coordinates
	Procedure G91();	// use relative coordinates
	Procedure M83();	// extruder relative mode

	Procedure G92();	// set position of axes

	Procedure G20();	// set units to inches
	Procedure G21();	// set units to millimeters

	Procedure M3();		// spindle on
	Procedure M5();		// spindle off

	Procedure G53();	// move in machine coords
	Procedure G54();	// to G59, set workspace coords or change to workspace

	Procedure M17();	// steppers on
	Procedure M18();	// steppers off

	Procedure M203();	// set max feedrate
	Procedure M92();	// set steps per unit
	Procedure G94();	// ^

	Procedure G27();	// park toolhead
	Procedure G28();	// auto-home

	Procedure M0();		// stop
	Procedure M112();	// emergency stop!
	Procedure M410();	// stop steppers now
	Procedure M2();		// end of program
	Procedure M108();	// resume

	Procedure M400();	// wait for moves to finish

}

#endif