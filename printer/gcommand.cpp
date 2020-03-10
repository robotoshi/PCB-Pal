#include "Arduino.h"
#include "gcommand.h"

GCommand::GCommand(char* line) : codes_len(0) {
	char* g = strtok(line, " ;\n");

	while (g) {
		codes[codes_len++] = parse_gcode(g);
		g = strtok(NULL, " ;\n");
	}
}

GCommand::~GCommand() {
	for (int i=0; i<codes_len; ++i)	delete codes[i];
}

GCode* GCommand::parse_gcode(char* str) {
	GCode* gc = new GCode;
	char letter = str[0];
	if (!isupper(letter)) {
		Serial.println("\n??"); // bad or is an argument
		gc->letter = '\0';
		gc->type = TEXT;
		strcpy(gc->number.text, str);
		return gc;
	} else {
		gc->letter = letter;
		++str;
	}

	// char* decimal = nullptr;

	// for (int i=0; str[i]; ++i) {
	// 	if (!isdigit(str[i])) {
	// 		if (str[i] == '.') decimal = str+i;
	// 		else return nullptr;
	// 	}
	// }

	if (letter == 'M' || letter == 'G') {
		gc->number.code = atoi(str);
		gc->type = CODE;
	} //else if (letter == 'T') ;
	else {
		gc->number.value = atof(str);
		gc->type = VALUE;
	}

	return gc;
}

void GCommand::dump() {
	for (int i=0; i<codes_len; ++i) {
		Serial.print(codes[i]->letter);

		switch(codes[i]->type) {
			case CODE:
				Serial.print(codes[i]->number.code);
				break;
			case VALUE:
				Serial.print(codes[i]->number.value);
				break;
			case TEXT:
				Serial.print("\"");
				Serial.print(codes[i]->number.text);
				Serial.print("\"");
				break;
		}
		Serial.print(" ");
	}
}

void GCommand::dumpln() {
	for (int i=0; i<codes_len; ++i) {
		Serial.print(codes[i]->letter);
		Serial.print(" ");

		switch(codes[i]->type) {
			case CODE:
				Serial.println(codes[i]->number.code);
				break;
			case VALUE:
				Serial.println(codes[i]->number.value);
				break;
			case TEXT:
				Serial.print("\"");
				Serial.print(codes[i]->number.text);
				Serial.println("\"");
				break;
		}
	}
}