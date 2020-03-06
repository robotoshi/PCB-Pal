#ifndef H_GCOMMAND
#define H_GCOMMAND

#include "Arduino.h"

union Digits {
	char text[12];
	int code;
	double value;
};

// what type of number comes after the first letter
enum Numtype {
	CODE,
	VALUE,
	TEXT
};

struct GCode {
	char letter;
	Digits number;
	Numtype type;
};

class GCommand {
private:
	GCode* codes[5];
	int codes_len;

	GCode* parse_gcode(char* str);		// split a gcode line into a GCommand object

public:
	GCommand() {}
	GCommand(char* line);

	char get_letter() { return codes[0]->letter; }
	int get_code() { return codes[0]->number.code; }
	void dump();
	void dumpln();
};

#endif