#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

//#define k 5

void interruptSigHandler (int);


struct Apple {
    int intendedNode;
    char message[100];
};

//*******************************************
//           DIAGNOSTIC MESSAGE
//*******************************************


int main() {

    struct Apple apple;


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

    //**************************************************
    //                  HANDLING CTRL+C
    //**************************************************
    if(signal(SIGINT, interruptSigHandler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }


    //while(getchar() != '\n' && getchar() != EOF);
    while(1) {
	

    	printf("Enter recipient node (int): ");
    	scanf("%d", &apple.intendedNode);

	//**************************************************
        //                PIPING
        //*************************************************
	int pipes[k][2];
    	for (int i = 0; i < k; i++) {
		if (pipe(pipes[i]) == -1) {
                    perror("pipe failure");
                    exit(1);
            	}
    	}
    	if (apple.intendedNode < 0 || apple.intendedNode > k){
        	printf("Error: Recipient Node must be between 0 & %d\n", k);
        	exit(1);
    	}

    	while (getchar() != '\n' && getchar() != EOF);

    	printf("Enter message (string): ");
    	fgets(apple.message, sizeof(apple.message), stdin);
    	char *newline = strchr(apple.message, '\n');
    	if (newline) {
        	*newline = '\0';
    	}
	
	//fork children each round
	for (int i = 0; i < k; i++) {
	    pid_t pid = fork();

	    if (pid < 0) {
		    perror("fork failure");
	            exit(1);
	    }

	    if (pid == 0) {
		    //child
		    int id = i;
		    int prev = (id - 1 + k) % k; 
		    int next = id;

		    struct Apple apple;

		    read(pipes[prev][0], &apple, sizeof(apple));

		    if (apple.intendedNode == id) {
			    printf("node %d has the apple", id);
			    printf("node %d received message %s\n", id, apple.message);
			    apple.intendedNode = -1; //node set to empty
			    strcpy(apple.message, "empty");
		    }

		    write(pipes[next][1], &apple, sizeof(apple));

		    exit(0);
	    }   
    }

    write(pipes[0][1], &apple, sizeof(apple));

    for (int i = 0; i < k; i++) {
	    wait(NULL);
    }
    }

    return 0;

}

void
interruptSigHandler (int sigNum){
    printf( " received. Shutting down...\n");
    exit(0);
}


