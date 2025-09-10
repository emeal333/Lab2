// Bekah Doody & Liz Mealing
// Lab 2 - 9.10.25

/* The Below is from sampleProgram4
 *
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
 if (argc < 3) {
 fputs("Usage: must supply a command\n", stderr);
 exit(1);
 }
 puts("Before the exec");
 if (execvp(argv[1], &argv[1]) < 0) {
 perror("exec failed");
 exit(1);
 }
 puts("After the exec");
 return 0;
}
*
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
    // Repeat until user enters quit
    while (1) {

    //Display a prompt to the user
    char command[256];
    printf("Enter a command: ");


    //Recieve and parse the user input
    //REad a line into a character array (preferably using fgets())
    if (fgets(command, sizeof(command), stdin) == NULL){
        printf("\n");
        break;
    }

    command[strcspn(command, "\n")] = 0; //removes \n char

    //Repeat until user enters quit
    if (strcmp(command, "quit") == 0) {
        break;
    }

    //Depending on your implementation:
    //  You can tokenize the line using your own custom function or by using strtok()
    //      word_1 = strtok (line, " ");
    //      word_2 = strtok (NULL, " ");
    //  or by using strsep()
    //      word_1 = strsep (&lineptr, " ");
    //      word_2 = strsep (& lineptr, " ");
    //
    
    char *token;
    char *args[64];
    int i =0;

    token = strtok(command, " \t\n");
    while (token != NULL && i < 63) {
        // printf("%s\n", token);
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;

    //Spawn a child process to execute the command
    pid_t pid;
    pid = fork();

    //  preferably use execvp() / execve() as in Sample Program 4 of this lab. Note: you must create and pass a vector of pointers to arguments
    //  or use execlp() / execle() as in the example call below. Note: in this case you must pass a fixed list of arguments.
    //      execlp ("prog_name", "prog_name", ARG, (char *) 0);
    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0 ) {
        //child process
        // create and pass a vector of pointers to arguments
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    } else {
        // parent process
        int status;
        struct rusage usage;

        if (wait4(pid, &status, 0, &usage) == -1){
            perror("wait4 failed");
            continue;
        }

        printf("\n .......Process Statistics........\n");
        printf("User CPU time userd: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
        printf("...................................\n\n");
    }
    }
   
    printf("Goodbye!\n");
    return 0;
}
    
    // Find and use the appropriate system call to collect resource usage statistics about each executed process
        // Output the "user CPU time used" for each individual child process spawned by the shell
        // Output the number of "involuntary context switches" experienced by each individual child process spawned by the shell
    // Repeat until the user enters "quit" 
    
