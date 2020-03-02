#include "Arduino.h"
#include "gcodes.h"
#include "gcommand.h"

static GCommand command;

/**
	Choose which function to call
*/
Procedure gcodes::dispatch(GCommand comm) {
	command = comm;
	Serial.println("\nDispatching...");

	char first_letter = command.get_letter();

	if (first_letter == 'G') {
		switch (command.get_code()) {
			case 0:  return G0;  break;
			case 1:  return G1;  break;
			case 90: return G90; break;
			case 91: return G91; break;
			case 92: return G92; break;
			case 20: return G20; break;
			case 21: return G21; break;
			case 53: return G53; break;
			case 54: return G54; break;
			case 94: return G94; break;
			case 27: return G27; break;
			case 28: return G28; break;
			default: return other;
		}
	} else if (first_letter == 'M') {
		switch (command.get_code()) {
			case 73:  return M73;  break;
			case 115: return M115; break;
			case 83:  return M83;  break;
			case 3:   return M3;   break;
			case 5:   return M5;   break;
			case 17:  return M17;  break;
			case 18:  return M18;  break;
			case 203: return M203; break;
			case 92:  return M92;  break;
			case 0:   return M0;   break;
			case 112: return M112; break;
			case 2:   return M2;   break;
			case 108: return M108; break;
			case 400: return M400; break;
			default:  return other;
		}
	} else return other;
}

Procedure gcodes::test() {
	static int count = 0;
	Serial.println("----");
	delay(200);
	if (count++ >= 10) {
		count = 0;
		return NULL;
	}
	return test;
}

Procedure gcodes::echo() {
	Serial.println("\n------");
	command.dump();
	Serial.println("------\n");
	return NULL;
}

Procedure gcodes::other() {
	Serial.println("Unknown Code");
	return NULL;
}

//////////////////////////////////////////////////////////////

Procedure gcodes::G0() { return test; }
Procedure gcodes::G1() { return echo; }

Procedure gcodes::M73() { return echo; }

Procedure gcodes::M115() { 
	Serial.println("PCB-Pal 1.0a");
	return NULL; 
}

Procedure gcodes::G90() { return echo; }
Procedure gcodes::G91() { return echo; }
Procedure gcodes::M83() { return echo; }

Procedure gcodes::G92() { return echo; }

Procedure gcodes::G20() { return echo; }
Procedure gcodes::G21() { return echo; }

Procedure gcodes::M3() { return echo; }
Procedure gcodes::M5() { return echo; }

Procedure gcodes::G53() { return echo; }
Procedure gcodes::G54() { return echo; }

Procedure gcodes::M17() { return echo; }
Procedure gcodes::M18() { return echo; }

Procedure gcodes::M203() { return echo; }
Procedure gcodes::M92() { return echo; }
Procedure gcodes::G94() { return echo; }

Procedure gcodes::G27() { return echo; }
Procedure gcodes::G28() { return echo; }

Procedure gcodes::M0() { return echo; }
Procedure gcodes::M112() { return echo; }
Procedure gcodes::M410() { return echo; }
Procedure gcodes::M2() { return echo; }
Procedure gcodes::M108() { return echo; }

Procedure gcodes::M400() { return echo; }