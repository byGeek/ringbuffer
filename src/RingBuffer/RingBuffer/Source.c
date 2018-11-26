#include "myfifo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
	myfifo_t* ptr= myfifo_create(10);

	printf("fifo empty: %s\n", myfifo_empty(ptr) ? "true" : "false");
	printf("fifo full: %s\n", myfifo_full(ptr) ? "true" : "false");

	myfifo_pop(ptr, "hello", 5);

	printf("fifo empty: %s\n", myfifo_empty(ptr) ? "true" : "false");

	char buffer[128];
	myfifo_push(ptr, buffer, 5);
	printf("fifo empty: %s\n", myfifo_empty(ptr) ? "true" : "false");

	myfifo_pop(ptr, "helloworld", 10);
	printf("fifo full: %s\n", myfifo_full(ptr) ? "true" : "false");

	memset(buffer, 0, sizeof(buffer));

	myfifo_push(ptr, buffer, 6);
	printf("fifo full: %s\n", myfifo_full(ptr) ? "true" : "false");

	myfifo_free(ptr);

}