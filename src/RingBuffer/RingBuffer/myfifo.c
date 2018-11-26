
#include "myfifo.h"
#include <stdlib.h>
#include <string.h>

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
int myfifo_push(myfifo_t* fifo, char* buffer, int len) {
	if (myfifo_empty(fifo) || len <= 0) return 0;

	unsigned int till_end = fifo->size - get_mask_pos(fifo->out, fifo->size);
	unsigned int used = myfifo_get_used_space(fifo);
	unsigned int ret = _min(used, len);
	char* out_pos = fifo->buf + get_mask_pos(fifo->out, fifo->size);
	char* in_pos = fifo->buf + get_mask_pos(fifo->in, fifo->size);

	if (len <= till_end) {
		memcpy(buffer, out_pos, ret);
	}
	else
	{
		unsigned int from_start = _min(len - till_end, get_mask_pos(fifo->in, fifo->size));
		memcpy(buffer, out_pos, till_end);
		memcpy(buffer + till_end, fifo->buf, from_start);
		ret = till_end + from_start;
	}

	fifo->out += ret;
	return ret;
}

//put data to fifo
//return actual bytes transffered
int myfifo_pop(myfifo_t* fifo, const char* buffer, int len) {
	if (len + myfifo_get_used_space(fifo) > fifo->size) {
		//overflow, do nothing
		return 0;
	}
	else {
		unsigned int ret = 0;
		char* out_pos = fifo->buf + get_mask_pos(fifo->out, fifo->size);
		char* in_pos = fifo->buf + get_mask_pos(fifo->in, fifo->size);

		if (get_mask_pos(fifo->in, fifo->size) + len <= fifo->size) {
			memcpy(in_pos, buffer, len);
			ret = len;
		}
		else {
			unsigned int till_end = fifo->size - get_mask_pos(fifo->in, fifo->size);
			memcpy(in_pos, buffer, till_end);
			memcpy(fifo->buf, buffer + till_end, len - till_end);
			ret = len;
		}

		fifo->in += ret;
		return ret;
	}
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