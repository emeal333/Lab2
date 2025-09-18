#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* workerThreads(void* arg);

int main () {

    char* fileName[200];
    printf("Please enter a fileName: ");
    scanf("%s", fileName);
    
    pthread_t thread1;
    int threadStatus1;
    

    threadStatus1 = pthread_create (&thread1, NULL, workerThreads, &fileName);
    if (threadStatus1 != 0){
        fprintf (stderr, "Thread create error %d: %s\n", threadStatus1, strerror(threadStatus1));
        exit (1);
    }
   
    
    


    return 0;
}

void* workerThreads(void* file){
    char *fileName = (char *)file;
    int sleepTimes[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 7, 8, 9, 10};
    int randomNumber = (rand() %20); //random number from 0 - 19
    sleep(sleepTimes[randomNumber]);

    //AWAKEN
    if (sleepTimes[randomNumber] > 2){
        printf("File [%s] has not been found", fileName);
    }
    else {
        printf("File [%s] has been found", fileName);
    }
    

    
    return NULL;
}

