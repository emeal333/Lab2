#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define FOO 4096

int main ()
{
    int shmId; //shared mem ID
    char *sharedMemoryPtr; //pointer to shared Memory

    if((shmId = shmget(IPC_PRIVATE, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        //set shmId to shmget which returns a resource ID, which can be used by any process wishing to access the shared segment
        perror ("Unable to get shared memory\n");
        exit (1);
        //just checking if shared mem was created
    }

    //set the ptr to shmar which attaches the specified shared memory region to the address space of the calling process
    if((sharedMemoryPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("Unable to attach\n");
        exit (1);
    }
    
    //printing the shared memory region to the address space of the calling process 
    // B = ptr + F00 (F00 = 3840 in decimal)
    printf("Value a: %p\t Value b: %p\n", (void *) sharedMemoryPtr, (void *)
    sharedMemoryPtr + FOO);

    if(shmdt (sharedMemoryPtr) < 0) {
        perror ("Unable to detach\n");
        exit (1);
    }

    struct shmid_ds stats;

    if (shmctl(shmId, IPC_STAT, &stats) < 0){
        perror ("shmctl failed");
        exit(1);
    }

    printf("Shared memory segment size: %lu bytes\n", stats.shm_segsz);

    if(shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("Unable to deallocate\n");
        exit(1);
    }

    return 0;
}
