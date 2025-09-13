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
};

int main() {

    struct Apple messenger;
    
    //Receiving user input
    printf("Enter recipient node (int): ");
    scanf("%d", &messenger.intendedNode);
    
    while (getchar() != '\n' && getchar() != EOF);

    printf("Enter message (string): ");
    fgets(messenger.message, sizeof(messenger.message), stdin);
//

    printf("Message: %s\n", messenger.message);
    printf("Intended Node: %d\n", messenger.intendedNode);


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


