#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define FOO 4096

int main () {
	key_t key;
	int shmID;
	char *sharedMemoryPtr;
	
	key = ftok("sharedKey", 65);
	
	if ((shmID = shmget(key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
 		perror ("Unable to get shared memory\n");
		exit(1);
	}

	//attach shared memory
	if ((sharedMemoryPtr = shmat (shmID, 0, 0)) == (void*) - 1) {
		perror("unable to attach\n");
		exit(1);
	}

	while(1) {
		char input[FOO];

		printf("your string input: ");
		fgets(input, FOO, stdin);

		strcpy(sharedMemoryPtr, input);
		printf("input %s written to shared memory", sharedMemoryPtr);
	}

	//printf("input written to shared memory %s\n", sharedMemoryPtr);
	return 0;
}	


