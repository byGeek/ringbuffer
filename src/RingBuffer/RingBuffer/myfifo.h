#pragma once
#include <stdbool.h>

typedef struct _my_fifo_t {
	char* buf;
	unsigned int in;
	unsigned int out;
	unsigned int size;
} myfifo_t;

//init fifo
bool myfifo_init(myfifo_t* fifo, unsigned int size);

//free fifo
void myfifo_free(myfifo_t* fifo);

//get data from fifo to buffer
//return actual bytes transffered
int myfifo_get(myfifo_t* fifo, char* buffer, int len);

//put data to fifo
//return actual bytes transffered
int myfifo_set(myfifo_t* fifo, const char* buffer, int len);

//check if fifo empty
bool myfifo_empty(myfifo_t* fifo);

//check if fifo full
bool myfifo_full(myfifo_t* fifo);

int myfifo_get_used_space(myfifo_t* fifo);

