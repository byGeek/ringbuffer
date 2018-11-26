#pragma once
#include <stdbool.h>


/*
if used for single producer and single consumer envirronment
it can be lock-free

This implementation will discard new data if overflow occur.

*/
typedef struct _my_fifo_t {
	char* const buf;  //inner buffer
	unsigned int in;  //indicate write index
	unsigned int out;  //indicate read index
	const unsigned int size;  //capacity
} myfifo_t;

//init fifo
myfifo_t* myfifo_create(unsigned int size);

//free fifo
void myfifo_free(myfifo_t* fifo);

//get data from fifo to buffer
//return actual bytes transffered
int myfifo_push(myfifo_t* fifo, char* buffer, int len);

//put data to fifo
//return actual bytes transffered
int myfifo_pop(myfifo_t* fifo, const char* buffer, int len);

//check if fifo empty
bool myfifo_empty(myfifo_t* fifo);

//check if fifo full
bool myfifo_full(myfifo_t* fifo);

int myfifo_get_used_space(myfifo_t* fifo);

