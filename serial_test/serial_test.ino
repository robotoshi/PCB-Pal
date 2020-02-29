#include "queue.h"
#include <string.h>

#define BAUD_RATE   9600
#define CMD_WID     50
#define QUEUE_LEN   5

#define DUMMY_TIME 3000		// time a dummy command takes to complete

Queue queue;
char inbuf[CMD_WID];
int index = 0;

long start_time = 0;
bool pending = false;

bool printing = false;
void (*task)();

void setup() {
	queue = Queue(QUEUE_LEN, CMD_WID);
	Serial.begin(BAUD_RATE);
	while (!Serial) ; 		// wait for serial to be ready
	task = T_request_gcode;
}

void loop() {
	task();						// current task to run
	if (printing) execute();	// we often want to be printing concurrently
}

/**
Ask for a gcode
*/
void T_request_gcode() {
	if (queue.vacancies() > 0) {
		Serial.print("READY ");							// ask for a gcode
		Serial.println(queue.vacancies());				// debug
		while (Serial.available()) Serial.read();		// ignore anything we didn't ask for so the buffer doesn't fill up
		task = T_receive_gcode;
	}
	index = 0;
}

/**
Listen for a gcode
*/
void T_receive_gcode() {
	if (Serial.available() > 0 ) {
		char c = Serial.read();
		
		if (c == '\n' || c == ';') {		// End of command. Start grabbing it
			inbuf[index++] = '\0';
			Serial.println(inbuf);
			queue.push_back(inbuf);
			Serial.println("OK\n");

			printing = true;
			task = T_request_gcode;
		} else {
			inbuf[index++] = c;
		}
	}
}

/**
Run gcodes in the buffer
*/
void execute() {
	if (pending) {
		if (millis() > start_time + DUMMY_TIME) pending = false;
	} else {
		start_time = millis();
		pending = true;

		if (queue.items() <= 0) {
			Serial.println("Buffer empty!"); 		// debug
			printing = false;						// dev
		} else {
			Serial.print("\t\t");
			Serial.print(queue.items());
			Serial.print(" EXEC: ");
			Serial.println(queue.pop_front());
		}
	}
}

void queue_dump() {
	for (int i =0; i<QUEUE_LEN; i++)
		Serial.println(queue.get(i));
}