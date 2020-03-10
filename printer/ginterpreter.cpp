#include "Arduino.h"
#include "ginterpreter.h"
#include "gcommand.h"
#include "printhead.h"

GInterpreter::GInterpreter(Printhead& head) : printhead(head) {

}

/**
	Choose which method to run later
*/
bool GInterpreter::interpret(char* line) {
	if (command) delete command;
	command = new GCommand(line);
	first_time = true;
	Serial.println("Interpreting...\n");
	if ( !(method = get_method()) ) {
		Serial.print("Unknown Code: \t");
		command->dump();
		return done();
	}
	return true;
}

/**
	Execute the current command by delegating to a member function
*/
bool GInterpreter::execute() {
	if (!method) {
		Serial.println("__Null Pointer Error__");
		Serial.flush();
		return false;
	}
	if ( (this->*method)() ) return true;
	else return done();
}

/** 
	Pick a method
*/
Method GInterpreter::get_method() {
	char first_letter = command->get_first_letter();
	if (first_letter == 'G') {
		switch (command->get_first_code()) {
			case 0:  return &G0;
			case 1:  return &G1;
			case 90: return &G90;
			case 91: return &G91;
			case 92: return &G92;
			case 20: return &G20;
			case 21: return &G21;
			case 53: return &G53;
			case 54: return &G54;
			case 94: return &G94;
			case 27: return &G27;
			case 28: return &G28;
			default: return nullptr;
		}
	} else if (first_letter == 'M') {
		switch (command->get_first_code()) {
			case 73:  return &M73;
			case 115: return &M115;
			case 83:  return &M83;
			case 3:   return &M3;
			case 5:   return &M5;
			case 17:  return &M17;
			case 18:  return &M18;
			case 203: return &M203;
			case 92:  return &M92;
			case 0:   return &M0;
			case 112: return &M112;
			case 2:   return &M2;
			case 108: return &M108;
			case 400: return &M400;
			default:  return nullptr;
		}
	} else return nullptr;
}

//////////////////////////////////////////////////////////////////////
// 	G-Code Functions
//////////////////////////////////////////////////////////////////////

bool GInterpreter::G0() {
	set_move_xy(10.0);
	return printhead.tick();
}

bool GInterpreter::G1() {
	printhead.engage();
	set_move_xy(20.0);
	return printhead.tick();
}

bool GInterpreter::M73() { return echo(); }

bool GInterpreter::M115() { 
	Serial.println("PCB-Pal 1.0a");
	return false; 
}

bool GInterpreter::G90() { return echo(); }
bool GInterpreter::G91() { return echo(); }
bool GInterpreter::M83() { return echo(); }

bool GInterpreter::G92() { return echo(); }

bool GInterpreter::G20() { return echo(); }
bool GInterpreter::G21() { return echo(); }

bool GInterpreter::M3() { return echo(); }
bool GInterpreter::M5() { return echo(); }

bool GInterpreter::G53() { return echo(); }
bool GInterpreter::G54() { return echo(); }

bool GInterpreter::M17() { return echo(); }
bool GInterpreter::M18() { return echo(); }

bool GInterpreter::M203() { return echo(); }
bool GInterpreter::M92() { return echo(); }
bool GInterpreter::G94() { return echo(); }

bool GInterpreter::G27() { return echo(); }
bool GInterpreter::G28() { return echo(); }

bool GInterpreter::M0() { return echo(); }
bool GInterpreter::M112() { return echo(); }
bool GInterpreter::M410() { return echo(); }
bool GInterpreter::M2() { return echo(); }
bool GInterpreter::M108() { return echo(); }

bool GInterpreter::M400() { return echo(); }


//////////////////////////////////////////////////////////////////////
// 	Private Helper Functions
//////////////////////////////////////////////////////////////////////

/**
	Wrap up the gcode when it's finished and send info
*/
bool GInterpreter::done() {
	Serial.println("\nDONE");
	first_time = true;
	return false;
}

void GInterpreter::set_move_xy(double speed) {
	if (first_time) {
		if (command->get_len() < 3) {
			Serial.println("Bad Code: Too few params");
			return false;
		}

		const GCode& xcode = *command->get_codes()[1];
		const GCode& ycode = *command->get_codes()[2];

		if (xcode.letter != 'X' || ycode.letter != 'Y') {
			Serial.println("Bad Code: Wrong params");
			return false;
		}

		printhead.set_move(xcode.number.value, ycode.number.value, speed);
		first_time = false;
	}
}

//////////////////////////////////////////////////////////////////////
// 	Test Functions
//////////////////////////////////////////////////////////////////////

bool GInterpreter::test() {
	static int count = 10;
	if (first_time) {
		count = 10;
		first_time = false;
	}

	Serial.print(count);
	Serial.println("----");
	delay(400);
	if (count-- <= 0) {
		count = 10;
		return false;
	}
	return true;
}


bool GInterpreter::echo() {
	Serial.println("------");
	command->dumpln();
	Serial.println("------");
	return false;
}