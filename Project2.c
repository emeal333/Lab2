#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <string.h>

// #define PERMIT_COUNT 2

sem_t mixer;
sem_t pantry;
sem_t fridge;
sem_t bowl;
sem_t spoon;
sem_t oven;


const char* fridge_ingred[] = {"milk", "eggs", "butter"};
const char* pantry_ingred[] = {"flour", "sugar", "yeast", "baking soda", "salt", "cinnamon"};

const int fridge_num_items = 3;
const int pantry_num_items = 6;

const char* cookies_ingred[] = {"flour", "sugar", "milk", "butter"};
const int cookies_num_items = 4;
const char* pancakes_ingred[] = {"flour", "sugar", "baking soda", "salt", "egg", "milk", "butter"};
const int pancakes_num_items = 7;

typedef struct {
	char name[16];
	int cookies_ingred_taken[4];
	int pancakes_ingred_taken[7];
} Baker;

void* thread_callback(void *arg) {
	//char* thread_name = (char*)arg;
	Baker* baker = (Baker*)arg;

	// fridge cs begins
	sem_wait(&fridge);
	printf("%s is in the fridge\n", baker->name);

	//check if baker has items for cookies
	for (int i = 0; i < fridge_num_items; i++) {
		for (int j = 0; j < cookies_num_items; j++) {
			if (strcmp(fridge_ingred[i], cookies_ingred[j]) == 0 && baker->cookies_ingred_taken[j] == 0) {
				baker->cookies_ingred_taken[j] = 1;
				printf("%s took %s from fridge\n", baker->name, fridge_ingred[i]);
			}
		}
	}	
	
	printf("%s exiting the fridge\n", baker->name);
	
	sem_post(&fridge);     
        // fridge cs ends
	
	printf("%s exited the fridge\n", baker->name);
	

	// pantry cs begins
	sem_wait(&pantry);
	printf("%s is in the pantry\n", baker->name);

	// check if baker has items for cookies
        for (int i = 0; i < pantry_num_items; i++) {
                for (int j = 0; j < cookies_num_items; j++) {
                        if (strcmp(pantry_ingred[i], cookies_ingred[j]) == 0 && baker->cookies_ingred_taken[j] == 0) {
				baker->cookies_ingred_taken[j] = 1;
				printf("%s took %s from pantry\n", baker->name, pantry_ingred[i]);
                        }
                }
        }

	printf("%s exiting the pantry\n", baker->name);

        sem_post(&pantry);
        // pantry cs end

        printf("%s exited the pantry\n", baker->name);

	int complete = 1;

	for (int j = 0; j < cookies_num_items; j++) {
		if (baker->cookies_ingred_taken[j] == 0) {
			complete = 0;
			break;
		}
	}
	if (complete) {
		printf("%s is ready to mix cookies\n", baker->name);
	}

        return NULL;
}


int main() {
	int n;
	
	printf("Enter number of bakers: ");
	
	scanf("%d", &n);
	
	pthread_t* threads = malloc(sizeof(pthread_t) * n);
	//char** names = malloc(sizeof(char*) * n);
	
	Baker* bakers = malloc(sizeof(Baker) * n);

	sem_init(&fridge, 0, 2); //last input value is how many of that resource there is
	sem_init(&pantry, 0, 1); 

	//create n threads and bakers(chatGPT helped)
	for (int i = 0; i < n; i++) {
		snprintf(bakers[i].name, sizeof(bakers[i].name), "baker%d", i + 1);
		memset(bakers[i].cookies_ingred_taken, 0, sizeof(bakers[i].cookies_ingred_taken));
		memset(bakers[i].pancakes_ingred_taken, 0, sizeof(bakers[i].pancakes_ingred_taken));
		
		if (pthread_create(&threads[i], NULL, thread_callback, &bakers[i]) != 0) {
			perror("pthread_create");
			// join previously created threads, cleanup
			for (int k = 0; k < i; k++) pthread_join(threads[k], NULL);
			sem_destroy(&fridge);
			sem_destroy(&pantry);
			free(threads);
			free(bakers);
			
			return 1;
		}
	}
	
	//join n threads
	for (int i = 0; i < n; i++) {
		pthread_join(threads[i], NULL);
	}
	
	
	free(threads);
	free(bakers);

	sem_destroy(&fridge);
	sem_destroy(&pantry);

	return 0;
}

