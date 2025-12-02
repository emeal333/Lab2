#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

// #define PERMIT_COUNT 2

sem_t mixer;
sem_t pantry;
sem_t fridge;
sem_t bowl;
sem_t spoon;
sem_t oven;

/*
const char* fridge_ingred[] = {}
const char* pantry_ingred[] = {}

const int fridge_num_items = ;
const int pantry_num_items = ;
*/

void* thread_callback(void *arg) {
	char* thread_name = (char*)arg;
	
	// fridge cs begins
	sem_wait(&fridge);    
	
	printf("%s is in the fridge\n", thread_name);
	
	printf("%s exiting the fridge\n", thread_name);
	
	sem_post(&fridge);     
        // fridge cs ends
	
	printf("%s exited the fridge\n", thread_name);
	

	//pantry cs begins
	sem_wait(&pantry);

        printf("%s is in the pantry\n", thread_name);

	printf("%s exiting the pantry\n", thread_name);

        sem_post(&pantry);
        // pantry cs ends

        printf("%s exited the pantry\n", thread_name);

        return NULL;
}


int main() {
	int n;
	
	printf("Enter number of bakers: ");
	
	scanf("%d", &n);
	
	pthread_t* threads = malloc(sizeof(pthread_t) * n);
	char** names = malloc(sizeof(char*) * n);
	
	sem_init(&fridge, 0, 1); //last input value is how many of that resource there is
	sem_init(&pantry, 0, 1); 

	//create n threads
	for (int i = 0; i < n; i++) {
		names[i] = malloc(16);
		sprintf(names[i], "baker%d", i + 1);
		
		pthread_create(&threads[i], NULL, thread_callback, names[i]);
	}
	
	//join n threads
	for (int i = 0; i < n; i++) {
		pthread_join(threads[i], NULL);
		free(names[i]);
	}
	
	free(names);
	free(threads);
	
	sem_destroy(&fridge);
	sem_destroy(&pantry);

	return 0;
}

