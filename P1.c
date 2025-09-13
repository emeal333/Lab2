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

    int pipes[k][2];
    for (int i = 0; i < k; i++) {
	    if (pipe(pipes[i]) == -1) {
		    perror("pipe failure");
		    exit(1);
	    }
    }

    for (int i = 0; i < k; i++) {
	    pid_t pid = fork();

	    if (pid < 0) {
		    perror("fork failure");
	            exit(1)
	    }

	    if (pid == 0) {
		    int id = i;
		    int prev = (id - 1 + k) % k; //chatgpt
		    int next = id;

		    strucy Apple apple;

		    read(pipes[prev][0], &apple, sizeof(apple));

		    if (apple.intendedNode == id) {
			    printf("node PID%d received message %s\n", id, apple.message);
			    apple.intendedNode = -1; //node set to empty
			    strcpy(apple.message, "empty");
		    }

		    write(pipes[next][1], &apple, sizeof(apple));

		    ext(0);
	    }
    }

    write(pipes[0][1], &messenger, sizeof(messenger));

    for (int i = 0; i < k; i++) {
	    wait(NULL);
    }

    void interruptSigHandler (int sigNum) {
	    printf( "received. Shutting down...\n");                            exit(0);
    }
	


    return 0;

}

void
interruptSigHandler (int sigNum){
    pinterruptSigHandler (int sigNum){
    printf( "received. Shutting down...\n");
    exit(0);
}


