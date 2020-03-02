#include "Arduino.h"
#include "gcommand.h"

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

	// char* decimal = NULL;

	// for (int i=0; str[i]; ++i) {
	// 	if (!isdigit(str[i])) {
	// 		if (str[i] == '.') decimal = str+i;
	// 		else return NULL;
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

GCommand::GCommand(char* line) {
	char* g = strtok(line, " ;\n");

	codes_len = 0;
	while (g) {
		codes[codes_len++] = parse_gcode(g);
		g = strtok(NULL, " ;\n");
	}
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