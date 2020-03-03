#include "queue.h"
#include "Arduino.h"

Queue::Queue (int l, int w) {
	len = l;
	width = w;
	back = 1;
	front = 0;
	size = 0;
	data = new char*[len];
	for (int i=0; i<len; ++i) data[i] = new char[width];
}

char* Queue::push_back(const char* in) {
	if (size >= len) return NULL;
	++size;
	if (--back < 0) back = len-1;
	strcpy(data[back], in);
	return in;
}

char* Queue::pop_front() {
	if (size <= 0) return NULL;
	--size;
	char* s = data[front];
	if (--front < 0) front = len-1;
	return s;
}

char* Queue::peek_front() {
//    if (size <= 0) return NULL;
	return data[front];
}