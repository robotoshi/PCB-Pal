#include <string.h>
#include "queue.h"
#include "gcommand.h"
#include "gcodes.h"

#define BAUD_RATE	9600
#define RX_TIMEOUT	3000
#define URGENT_INTERVAL 500

#define CMD_WID		50
#define QUEUE_LEN	5

Queue queue;
char inbuf[CMD_WID];
int requested = 0;
int index = 0;

bool urgent = false;
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
	static unsigned long prev_rx = millis();

	if (!urgent && millis() > prev_rx + URGENT_INTERVAL) {
		if (check_urgent(Serial.peek())) {
			Serial.read();
			task = urgent_process;
		}
		prev_rx = millis();
	}

	Procedure t = task();
	if (t == NULL) task = (urgent) ? urgent_process : receive;		// escapse task
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
		return receive;
	} else return load_gcode;
}


/**
	Alternate receive/load function for when "urgent mode" has been 
	triggered with '<' and escaped with '>'
*/
Procedure urgent_process() {
	char c;
	if (Serial.available() > 0 ) {
		if (Serial.peek() == '>') {
			if (index == 0) {
				urgent = false;
				Serial.println("\nNormal Mode>");
				c = Serial.read();
				return NULL;
			} else 		// if the '>' is not on a line of its own, end this line and pretend it was
				c = '\n';
		} else 
			c = Serial.read();

		if (c == '\n' || c == ';') {
			if (index <= 0) return NULL;
			inbuf[index] = '\0';
			Serial.println();
			Serial.println(inbuf);
			index = 0;

			// if the first letter is not a letter, it's a bad code
			if (!isupper(inbuf[0])) {
				Serial.println("<^Invalid Command^\n");
				return NULL;
			}

			Serial.println("OK");
			Serial.print("\t\t<EXEC: ");
			Serial.println(inbuf);
			// ask dispatch() to fetch us the right Procedure for this command
			return gcodes::dispatch(GCommand(inbuf));

		} else {
			inbuf[index++] = c;
		}
	}
	return urgent_process;
}

/**
	Check for '<' and set the urgent flag
*/
bool check_urgent(char c) {
	if (c == '<') {
		urgent = true;
		Serial.println("\n<Urgent Mode");
		index = 0;
		return true;
	}
	return false;
}

/**
	Listen for a gcode
*/
Procedure receive() {
	static long prev_rx_time;
	static bool waiting = false;

	if (urgent) return urgent_process;

	if (!waiting) {
		waiting = true;
		prev_rx_time = millis();
	}

	if (requested <= 0) {
		index = 0;
		return request_gcode;
	}

	if (Serial.available() > 0 ) {
		prev_rx_time = millis();
		char c = Serial.read();

		if (check_urgent(c)) return urgent_process;
		
		if (c == '\n' || c == ';') {		// End of command. Start grabbing it
			if (index <= 0) return receive;
			inbuf[index++] = '\0';
			Serial.println();
			Serial.println(inbuf);

			if (!isupper(inbuf[0])) {
				Serial.println("^Invalid Command^\n");
				index = 0;
				return receive;
			}
			
			// add this to the queue			
			queue.push_back(inbuf);
			Serial.println("OK\n");

			--requested;
			waiting = false;
			index = 0;
			return request_gcode;
		} else {
			inbuf[index++] = c;
		}
	} else if (waiting && millis() > prev_rx_time + RX_TIMEOUT) { 	// timeout waiting for a response
		waiting = false;
		index = 0;
		return load_gcode;
	}
	return receive;
}

/**
	Run the next gcode in the queue
*/
Procedure load_gcode() {
	if (queue.items() <= 0) {
		Serial.println("Queue empty!"); 		// debug
		return receive;
	} else {
		Serial.print("\t\t");
		Serial.print(queue.items());
		Serial.print(" EXEC: ");
		char* line = queue.pop_front();
		Serial.println(line);
		return gcodes::dispatch(GCommand(line));
	}
}

void queue_dump() {
	for (int i=0; i<QUEUE_LEN; i++)
		Serial.println(queue.get(i));
}