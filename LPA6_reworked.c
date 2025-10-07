#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/sem.h>
#define SIZE 16


typedef struct {
    long int values[SIZE];
    sem_t semaphore;
} SharedMemory;

int main(int argc, char *argv[])
{
    int status;
    long int i, loop, temp; 
    int sharedMemoryID;
    SharedMemory *sharedMemoryPointer;
    pid_t pid;

    if (argc < 2) {
        printf("%s error\n", argv[0]);
        exit(1);
    }

    loop = atoi(argv[1]);
    

    sharedMemoryID = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT|S_IRUSR|S_IWUSR);
    if(sharedMemoryID < 0) {
        perror ("Unable to obtain shared memory\n");
        exit (1);
    }

    //sharedMemoryPointer = shmat(sharedMemoryID, 0, 0);

    /*if(sharedMemoryPointer == (void*) -1) {
        perror ("Unable to attach\n");
        exit (1);
    }*/

    sharedMemoryPointer = (SharedMemory *) shmat(sharedMemoryID, 0, 0);
	if (sharedMemoryPointer == (void*) -1) {
        	perror("Unable to attach shared memory");
        	exit(1);
    }

    sharedMemoryPointer->values[0] = 0;
    sharedMemoryPointer->values[1] = 1;
    
    if (sem_init(&sharedMemoryPointer->semaphore, 1, 1) == -1) {
        perror("failed");
        exit(1);
    }
    
    
    pid = fork();

    if(pid < 0){
        printf("Fork failed\n");
    }

    if(pid == 0) { // Child
        for(i=0; i<loop; i++) {
		sem_wait(&sharedMemoryPointer->semaphore); //lock
            // swap the content
            
            //if(semop(sharedMemoryID, xx, xx) < 0){
              //  perror("semop failed");
                //exit(1);
            //}


            temp = sharedMemoryPointer->values[1];
            
            sharedMemoryPointer->values[1] = sharedMemoryPointer->values[0];
            sharedMemoryPointer->values[0] = temp;

	    sem_post(&sharedMemoryPointer->semaphore);//unlock
            
        }

	shmdt(sharedMemoryPointer);
	exit(0);
    } else {
	    for(i=0; i<loop; i++) {
		    sem_wait(&sharedMemoryPointer->semaphore); //lock
							       //
		    // swap the contents of sharedMemoryPointer[1] and sharedMemoryPointer[0]
		    temp = sharedMemoryPointer->values[1];
		    sharedMemoryPointer->values[1] = sharedMemoryPointer->values[0];
		    sharedMemoryPointer->values[0] = temp;
		    
		    sem_post(&sharedMemoryPointer->semaphore);//unlock
	    }
	    
	    wait(&status);
	    printf("Values: %li\t%li\n", sharedMemoryPointer->values[0], sharedMemoryPointer->values[1]);

	    	
	    sem_destroy(&sharedMemoryPointer->semaphore);
	    
	    if(shmdt(sharedMemoryPointer) < 0) {
            	perror ("Unable to detach\n");
            	exit (1);
        }

	    if (shmctl(sharedMemoryID, IPC_RMID, NULL) < 0) {
            perror("Unable to remove shared memory");
            exit(1);
        }


        exit(0);
    }

    return 0;
}

