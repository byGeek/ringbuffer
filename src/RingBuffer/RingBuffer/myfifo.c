
#include "myfifo.h"
#include <stdlib.h>
#include <string.h>

#define MASK_POS(x, y) ((x)%(y))
static unsigned int get_mask_pos(unsigned int pos, unsigned int size) {
	return pos % size;
}

static int _min(int a, int b) {
	return a <= b ? a : b;
}

//init fifo
myfifo_t* myfifo_create(unsigned int size) {
	char* temp = malloc(size);
	if (temp == NULL) {
		return NULL;
	}
	else {
		/*
		in order to initialize const member in struct
		memcpy a exist myfifo_t obj
		ref:
		https://stackoverflow.com/questions/2219001/how-to-initialize-const-members-of-structs-on-the-heap
		*/
		myfifo_t fifo = { temp, 0, 0, size };
		myfifo_t* ret = malloc(sizeof(myfifo_t));
		memcpy(ret, &fifo, sizeof(myfifo_t));
		return ret;
	}
}

//free fifo
void myfifo_free(myfifo_t* fifo) {
	if (fifo != NULL && fifo->buf != NULL) {
		free(fifo->buf);
		free(fifo);
		//memset(fifo, 0, sizeof(myfifo_t));
	}
}

//get data from fifo to buffer
//return actual bytes transffered
int myfifo_pop(myfifo_t* fifo, char* buffer, int len) {
	if (myfifo_empty(fifo) || len <= 0) return 0;

	unsigned int till_end = fifo->size - MASK_POS(fifo->out, fifo->size);
	unsigned int avail = fifo->in - fifo->out;  //we don't allow overflow happen

	if (len > avail) {
		len = avail;
	}

	if (len <= till_end) {
		memcpy(buffer, fifo->buf + MASK_POS(fifo->out, fifo->size), len);
	}
	else {
		memcpy(buffer, fifo->buf + MASK_POS(fifo->out, fifo->size), till_end);
		memcpy(buffer + till_end, fifo->buf, len - till_end);
	}
	fifo->out += len;
	return len;
}

//put data to fifo
//return actual bytes transffered
int myfifo_push(myfifo_t* fifo, const char* buffer, int len) {

	if (len == 0) return 0;
	unsigned int avail = fifo->size - (fifo->in - fifo->out);

	if (len > avail) {
		len = avail;
	}

	unsigned int till_end = fifo->size - MASK_POS(fifo->in, fifo->size);

	if (len <= till_end) {
		memcpy(fifo->buf + MASK_POS(fifo->in, fifo->size), buffer, len);
	}
	else {
		memcpy(fifo->buf + MASK_POS(fifo->in, fifo->size), buffer, till_end);
		memcpy(fifo->buf, buffer + till_end, len - till_end);
	}

	fifo->in += len;
	return len;
}

//check if fifo empty
bool myfifo_empty(myfifo_t* fifo) {
	return (fifo->in == fifo->out) ? true : false;
}

//check if fifo full
bool myfifo_full(myfifo_t* fifo) {
	return (fifo->in - fifo->out == fifo->size) ? true : false;
}

int myfifo_get_used_space(myfifo_t* fifo) {
	return fifo->in - fifo->out;
}