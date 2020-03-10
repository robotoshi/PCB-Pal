#ifndef GCOMMAND_H
#define GCOMMAND_H

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
	static constexpr int max_codes = 6;
	GCode* codes[max_codes];	// shallow copied over nicely by default
	int codes_len;

	GCode* parse_gcode(char* str);		// split a gcode line into a GCommand object

public:
	GCommand(char* line);
	~GCommand();

	char get_first_letter() { return codes[0]->letter; }
	int get_first_code() { return codes[0]->number.code; }
	void dump();
	void dumpln();
	const GCode *const *const get_codes() { return codes; }
	int get_len() { return codes_len; }
};

#endif