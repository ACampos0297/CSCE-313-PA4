#ifndef BoundedBuffer_h
#define BoundedBuffer_h

#include <stdio.h>
#include <queue>
#include <string>
using namespace std;

class BoundedBuffer {
private:
	int capacity;
	pthread_cond_t full;
	pthread_cond_t empty;
	pthread_mutex_t m;
	queue<string> q;	
public:
    BoundedBuffer(int _cap);
	~BoundedBuffer();
	int size();
    void push (string);
    string pop();
};

#endif /* BoundedBuffer_ */
