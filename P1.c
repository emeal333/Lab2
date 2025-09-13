#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define k 5

void interruptSigHandler (int);


struct Apple {
    int intendedNode;
    char message[100];
};

//*******************************************
//           DIAGNOSTIC MESSAGE
//*******************************************


int main() {

    struct Apple messenger;


    //******************************************************
    //              RECEIVING USER INPUT
    //******************************************************
    int k;
    printf("Enter number of nodes (int): ");
    scanf("%d", &k);

   
    if (k <= 0){
        printf("Error: Number of nodes must be positive.\n");
        exit(1);
    }

    while(getchar() != '\n' && getchar() != EOF);

    printf("Enter recipient node (int): ");
    scanf("%d", &messenger.intendedNode);

    if (messenger.intendedNode < 0 || messenger.intendedNode > k){
        printf("Error: Recipient Node must be between 0 & %d\n", k);
        exit(1);
    }
    
    while (getchar() != '\n' && getchar() != EOF);

    printf("Enter message (string): ");
    fgets(messenger.message, sizeof(messenger.message), stdin);
    char *newline = strchr(messenger.message, '\n');
    if (newline) {
        *newline = '\0';
    }
//
   

    //**************************************************
    //                  HANDLING CTRL+C
    //**************************************************
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


