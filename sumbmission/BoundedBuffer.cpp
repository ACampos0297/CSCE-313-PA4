#include "BoundedBuffer.h"
#include <string>
#include <queue>
#include <iostream>
using namespace std;

BoundedBuffer::BoundedBuffer(int _cap) {
	this->capacity = _cap;
	//initialize mutex and conds
	pthread_cond_init(&full, NULL);
	pthread_cond_init(&empty, NULL);
	pthread_mutex_init(&m, NULL);
}

BoundedBuffer::~BoundedBuffer() {
	//destroy mutex and conds
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&empty);
	pthread_cond_destroy(&full);
	
	//remove queue
	queue<string> emptyQueue;
	swap(q,emptyQueue);
}

int BoundedBuffer::size() {
	//thread safe size
	pthread_mutex_lock(&m);
	int size = q.size();
	pthread_mutex_unlock(&m);
	return size;
}

void BoundedBuffer::push(string str) {
	/*
	Is this function thread-safe??? Does this automatically wait for the pop() to make room 
	when the buffer if full to capacity???
	*/
	pthread_mutex_lock(&m);
	//guard overflow (when size of queue = capacity of buffer)
	while(q.size()==this->capacity)
	{
		pthread_cond_wait(&full, &m);
	}
	q.push (str);
	pthread_cond_signal(&empty);
	pthread_mutex_unlock(&m);
}

string BoundedBuffer::pop() {
	/*
	Is this function thread-safe??? Does this automatically wait for the push() to make data available???
	*/
	pthread_mutex_lock(&m);
	//guard underflow
	while(q.size() == 0)
	{
		pthread_cond_wait(&empty, &m);
	}
	string s = q.front();
	q.pop();
	pthread_cond_signal(&full);
	pthread_mutex_unlock(&m);
	return s;
}
