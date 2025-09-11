//Bekah Doody & Liz Mealing
/*
 * Write a parent program that:
 *      Spawns off a child process
 *      Installs signal handler(s) for the two user-defined signals (SIGUSR1/SIGUSR2)
 *      When a user-defined signal is recieved, it reports the type of signal sent
 *          note* It may be necessary to reinstall your signal handler after a signal is recieved
 *      Terminated gracefully upon recieving a Control - C
 *
 *  The child process should repeatedly:
 *      Wait a random amount of time (1-5 seconds)
 *      Randomly send one of the two user-defined signals to its parent
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void SIGUSR1 (int);
void SIGUSR2 (int);

int main()
{
    //spawn off a child process
    int pid;
    pid = fork();

    if(pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if(pid==0)
    {
        //child

        //generates a random number - %5 gives num between 0 & 4, +1 is 0 & 5
        int randomNumber = (rand() %5) +1;        

        //random number for signal selection
        int randomNumber2 = (rand() %2);
        char signals[2][50] = {"SIGUSR1", "SIGUSR2"};
        
        signal (!!!!!!!SIGNAL TYPE!!!!!!!,signals[randomNumber2]);
        
        sleep(randomNumber);


    }
    else
    {
        //parent
    }
    return 0;
}

void SIGUSR1 (int sig){ 
    printf("recieved a SIGUSR1 signal.\n");
    

}

void SIGUSR2 (int sig){ 
    printf("recieved a SIGUSR2 signal.\n");


}

void SIGINT (int sigNum){
    printf("^C recieved. That's it. I'm shutting you down...");
    exit(0);
}




