#include <stdio.h>
#include <stdlib.h>

int global = 10; //global stored in initialized data
int global_un;

int main() {
	global_un = 20; //global_un stored uninitialized data
	int *ptr = (int*) malloc(sizeof(int) * 10); //create int pointer for heap segment
	int local = 30; //local for stack

	printf("Initialized data address: %p\n", (void *) &global);
	printf("Uninitialized data address: %p\n", (void *) &global_un);
	printf("Heap address: %p\n", (void *) ptr);
	printf("Stack address: %p\n", (void *) &local);
	printf("code address: %p\n", (void *) main);

	free(ptr);
	return 0;
}
