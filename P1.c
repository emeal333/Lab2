#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

void interruptSigHandler (int);


struct Apple {
    int intendedNode;
    char message[100];
}

int main() {

    int iterator = 0;
    
    //Receiving user input
    int recipient;
    printf("Enter recipient node (int): \n");
    fgets(recipient, sizeof(recipient), stdin);
    
    char message[500];
    printf("Enter message (string): \n");
    fgets(message, sizeof(message), stdin);
    //



    if(signal(SIGINT, interruptSigHandler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }



    return 0;

}

void
interruptSigHandler (int sigNum){
    printf( "received. Shutting down...\n");
    exit(0);
}


