#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	char userInput[1024];
	char *token;
	char *args[64];
	pid_t pid;
	int i = 0;

	printf("enter something: ");
	fgets(userInput, sizeof(userInput), stdin);

	// strip newline from input
	userInput[strcspn(userInput, "\n")] = '\0';
	
	token = strtok(userInput, " ");
	while (token != NULL) {
		// printf("%s\n", token);
		args[i++] = token;
		token = strtok(NULL, " ");
	}	
	args[i] = NULL;

	pid = fork();

	if (pid < 0) {
		perror("fork failed");
		return 1;
	} else if (pid == 0) {
		execvp(args[0], args); 
		perror("exec failed");
		exit(1);
	} else {
		wait(NULL);
	}

	return 0;
}

