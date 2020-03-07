#ifndef QUEUE_H
#define QUEUE_H

/**
My own app-specific queue class. 
Was it the most eficient way? Probably not. 
Does it use great coding practicies? No.
Does it work? Looks like it.
*/

class Queue {
private:
	
	int len;
	int width;
	int back;
	int front;
	int size;
	char** data;
	
public:

	Queue() {}
	Queue (int l, int w);

	char* push_back(const char* in);
	char* pop_front();
	char* peek_front();

	int vacancies() {return len - size; }
	int items() { return size; }
	char* get(int index) { return data[index]; }
};

#endif