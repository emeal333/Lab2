#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>


void sigHandler1 (int);
void sigHandler2 (int);

int main() {
    // different sequence of random numbers each time
    while(1){
    srand(time(NULL));
	pid_t pid;
    
    
	if (signal(SIGUSR1, sigHandler1) == SIG_ERR) {
		perror("signal SIGUSR2");
		exit(1);
	}

	if (signal(SIGUSR2, sigHandler2) == SIG_ERR) {
		perror("signal SIGUSR2");
		exit(1);
	}

	pid = fork();

	if (pid < 0) {
		perror("fork failed");
		exit(1);

	} else if (pid == 0) {
        	//child

        	//generates a random number - %5 gives num between 0 & 4, +1 is 0 & 5
        	int randomNumber = (rand() %5) +1;

        	//random number for signal selection
        	int randomNumber2 = (rand() %2);
        	//char signals[2][50] = {"SIGUSR1", "SIGUSR2"};
		
		//chatgpt sugested fix to make it give an array instead of strings 
		int signals[2] = {SIGUSR1, SIGUSR2};
		char *signalNames[2] = {"SIGUSR1", "SIGUSR2"};

        	//signal (!!!!!!!SIGNAL TYPE!!!!!!!,signals[randomNumber2]);

		printf("spawned child PID#: %d\n", getpid());
        printf("Random sleep time: %d\n", randomNumber);		
        sleep(randomNumber);
		kill(getppid(), signals[randomNumber2]);

		exit(0);
	} else {
		printf("parent waiting...\n");
		pause();
		wait(NULL);
		printf("child exited\n");

		//exit(0);
	}
    }
	return 0;
}

void 
sigHandler1 (int sigNum) {
	//
	printf("parent received sigusr1 %d\n", sigNum);
	//exit(0);
}

void
sigHandler2 (int sigNum) {
	printf("parent received sigusr2 %d\n", sigNum);
}
