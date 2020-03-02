#include <string.h>
#include "queue.h"
#include "gcommand.h"
#include "gcodes.h"

#define BAUD_RATE	9600
#define RX_TIMEOUT	3000

#define CMD_WID		50
#define QUEUE_LEN	5

Queue queue;
char inbuf[CMD_WID];
int requested = 0;
int index = 0;

Procedure task;

void setup() {
	queue = Queue(QUEUE_LEN, CMD_WID);
	Serial.begin(BAUD_RATE);
	while (!Serial) ; 		// wait for serial to be ready
	task = request_gcode;
	Serial.println('\0');
}


/**
	Each task will return a Procedure (function pointer) for what should be done next.
	Some things can't see the options, so they can send NULL and the escape task will be run
*/
void loop() {
	Procedure t = task();
	if (t == NULL) task = receive_gcode;		// escapse task
	else task = t;
}

/**
	Ask for a gcode
*/
Procedure request_gcode() {
	if (requested > 0 ) return load_gcode;

	if (queue.vacancies() > 0) {
		while (Serial.available()) Serial.read();		// ignore anything we didn't ask for so the buffer doesn't fill up
		Serial.print("READY ");							// ask for a gcode
		Serial.println(queue.vacancies());
		
		++requested;
		index = 0;
		return receive_gcode;
	} else return load_gcode;
}

/**
	Listen for a gcode
*/
Procedure receive_gcode() {
	static long start_time;
	static bool waiting = false;

	if (requested <= 0) return request_gcode;
	if (!waiting) {
		waiting = true;
		start_time = millis();
	}

	if (Serial.available() > 0 ) {
		start_time = millis();
		char c = Serial.read();
		
		if (c == '\n' || c == ';') {		// End of command. Start grabbing it
			inbuf[index++] = '\0';
			Serial.println();
			Serial.println(inbuf);

			queue.push_back(inbuf);
			Serial.println("OK\n");

			
			--requested;
			waiting = false;
			return request_gcode;
			
		} else {
			inbuf[index++] = c;
		}
	} else {
		if (waiting && millis() > start_time + RX_TIMEOUT) {
			waiting = false;
			return load_gcode;
		}
	}
	return receive_gcode;
}

/**
	Run the next gcode in the buffer
*/
Procedure load_gcode() {
	if (queue.items() <= 0) {
		Serial.println("Buffer empty!"); 		// debug
		return receive_gcode;
	} else {
		Serial.print("\t\t");
		Serial.print(queue.items());
		Serial.print(" EXEC: ");
		char* code = queue.pop_front();
		Serial.println(code);
		return gcodes::dispatch(GCommand(code));
	}
}

void queue_dump() {
	for (int i =0; i<QUEUE_LEN; i++)
		Serial.println(queue.get(i));
}