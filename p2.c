#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

// #define PERMIT_COUNT 2

//equipment & ingredients are stored in our kitchen!
typedef struct {
    sem_t mixer, pantry, fridge, bowl, spoon, oven;
    sem_t flour, sugar, yeast, baking_soda, salt, cinnamon;
    sem_t egg, milk, butter;
} Kitchen;

Kitchen kitchen;

const char* colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

// baker struct 
typedef struct {
	char name[16];
    int color;
    int ramsied;

	int flour, sugar, yeast, baking_soda, salt, cinnamon;
    int egg, milk, butter;
    int

    //recipes completed
    int cookies_baked, pancakes_baked, pizza_baked, pretzels_baked, rolls_baked;

    int cookies_ingred_taken, pancakes_ingred_taken, pretzels_ingred_taken, rolls_ingred_taken, pizza_ingred_taken; //boolean to show if they have the ingredients

} Baker;

int check_ramsied(Baker* baker) {
    if (baker->ramsied && (rand() % 100 < 15)) {
        printf("%s%s: RAMSIED! Dropping everything!\033[0m\n", colors[baker->color], baker->name)
        if (baker->flour) sem_post(&kitchen.flour);
        if (baker->sugar) sem_post(&kitchen.sugar);
        if (baker->yeast) sem_post(&kitchen.yeast);
        if (baker->baking_soda) sem_post(&kitchen.baking_soda);
        if (baker->salt) sem_post(&kitchen.salt);
        if (baker->cinnamon) sem_post(&kitchen.cinnamon);
        if (baker->egg) sem_post(&kitchen.egg);
        if (baker->milk) sem_post(&kitchen.milk);
        if (baker->butter) sem_post(&kitchen.butter);

        baker->flour = 0;
        baker->sugar = 0;
        baker->yeast = 0; 
        baker->baking_soda = 0;
        baker->salt = 0; 
        baker->cinnamon = 0; 
        baker->egg = 0; 
        baker->milk = 0; 
        baker->butter = 0;

        printf("%s%s: Starting over...\033[0m\n", colors[baker->color], baker->name)
        return 1; //because the Ramsied happened
    }
    return 0; //bc no ramsied happened
    
}

// initializing the fridge, pantry, and reciptes
const char* fridge_ingred[] = {"milk", "eggs", "butter"};
const char* pantry_ingred[] = {"flour", "sugar", "yeast", "baking soda", "salt", "cinnamon"};

const int fridge_num_items = 3;
const int pantry_num_items = 6;

const char* cookies_ingred[] = {"flour", "sugar", "milk", "butter"};
const int cookies_num_items = 4;
const char* pancakes_ingred[] = {"flour", "sugar", "baking soda", "salt", "egg", "milk", "butter"};
const int pancakes_num_items = 7;
const char* pizza_ingred[] = {"yeast", "sugar", "salt"};
const int pizza_num_items = 3;
const char* pretzels_ingred[] = {"flour", "sugar", "salt", "yeast", "baking soda", "egg"};
const int pretzels_num_items = 6;
const char* rolls_ingred[] = {"flour", "sugar", "salt", "butter", "eggs", "cinnamon"};


// int[] ingredient_check(Baker* baker)


void bake(Baker* baker){
    if (!baker->cookies_baked && baker->flour && baker->sugar && baker->milk && baker->butter) {
        printf("%s%s: I have all of the ingredients for COOKIES! Starting COOKIES now! \033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.bowl);
        sem_wait(&kitchen.spoon);
        sem_wait(&kitchen.mixer);

        printf("%s%s: I have all the eqipment for COOKIES! Mixing now!\033[0m\n", colors[baker->color], baker->name);

        sem_post(&kitchen.mixer);
        sem_post(&kitchen.spoon);
        sem_post(&kitchen.bowl);

        printf("%s%s: I've made the COOKIE dough! Ready to bake!\033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.oven);
        printf("%s%s: The COOKIES are in the oven!\033[0m\n", colors[baker->color], baker->name);
        sem_post(&kitchen.oven);
        
        baker->flour = 0;
        baker->sugar = 0;
        baker->milk = 0;
        baker->butter = 0;
        baker->cookies_baked = 1;
        
        printf("%s%s: C is for Cookie! Om nom nom nom...\033[0m\n", colors[baker->color], baker->name);
        printf("%s%s: Cookies are done!  %d/5 recipes are complete!\033[0m\n", colors[baker->color], baker->name, baker->cookies_baked + 
            baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked);
    }

    if (!baker->pancakes_baked && baker->flour && baker->sugar && baker->baking_soda && baker->salt && baker->egg && baker->milk && baker->butter) {
        printf("%s%s: I have all of the ingredients for PANCAKES! Starting PANCAKES now! \033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.bowl);
        sem_wait(&kitchen.spoon);
        sem_wait(&kitchen.mixer);

        printf("%s%s: I have all the eqipment for PANCAKES! Mixing now!\033[0m\n", colors[baker->color], baker->name);

        sem_post(&kitchen.mixer);
        sem_post(&kitchen.spoon);
        sem_post(&kitchen.bowl);

        printf("%s%s: I've made the PANCAKES batter! Ready to bake!\033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.oven);
        printf("%s%s: The PANCAKES are in the oven!\033[0m\n", colors[baker->color], baker->name);
        sem_post(&kitchen.oven);
        
        baker->flour = 0;
        baker->sugar = 0;
        baker->baking_soda=0;
        baker->salt = 0;
        baker->egg = 0;
        baker->milk = 0;
        baker->butter = 0;
        baker->pancakes_baked = 1;
           
        printf("%s%s: Somebody phone Canada and ask for their best maple syrup because...\033[0m\n", colors[baker->color], baker->name);
        printf("%s%s: PANCAKES are done!  %d/5 recipes are complete!\033[0m\n", colors[baker->color], baker->name, baker->cookies_baked + 
            baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked);
    }

    if (!baker->pizza_baked && baker->yeast && baker->sugar && baker->salt) {
        printf("%s%s: I have all of the ingredients for HOMEMADE PIZZA DOUGH! Starting PIZZA DOUGH now! \033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.bowl);
        sem_wait(&kitchen.spoon);
        sem_wait(&kitchen.mixer);

        printf("%s%s: I have all the eqipment for PIZZA DOUGH! Mixing now!\033[0m\n", colors[baker->color], baker->name);

        sem_post(&kitchen.mixer);
        sem_post(&kitchen.spoon);
        sem_post(&kitchen.bowl);

        printf("%s%s: I've made the PIZZA DOUGH! Ready to bake!\033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.oven);
        printf("%s%s: The PIZZA DOUGH is in the oven!\033[0m\n", colors[baker->color], baker->name);
        sem_post(&kitchen.oven);
        
        baker->sugar = 0;
        baker->yeast = 0;
        baker->salt = 0;
        baker->pizza_baked = 1;
           
        printf("%s%s: It's-a me...\033[0m\n", colors[baker->color], baker->name);
        printf("%s%s: PIZZA DOUGH is done!  %d/5 recipes are complete!\033[0m\n", colors[baker->color], baker->name, baker->cookies_baked + 
            baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked);
    }

    if (!baker->pretzels_baked && baker->yeast && baker->sugar && baker->salt && baker->flour && baker->baking_soda && baker->egg) {
        printf("%s%s: I have all of the ingredients for SOFT PRETZELS! Starting PRETZELS now! \033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.bowl);
        sem_wait(&kitchen.spoon);
        sem_wait(&kitchen.mixer);

        printf("%s%s: I have all the eqipment for PRETZELS! Mixing now!\033[0m\n", colors[baker->color], baker->name);

        sem_post(&kitchen.mixer);
        sem_post(&kitchen.spoon);
        sem_post(&kitchen.bowl);

        printf("%s%s: I've made the PRETZELS dough! Ready to bake!\033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.oven);
        printf("%s%s: The PRETZELS are in the oven!\033[0m\n", colors[baker->color], baker->name);
        sem_post(&kitchen.oven);
        
        baker->sugar = 0;
        baker->yeast = 0;
        baker->salt = 0;
        baker->flour = 0;
        baker->baking_soda = 0;
        baker->egg = 0;
        baker->pretzels_baked = 1;
           
        printf("%s%s: Twist and shout, they're fresh out...\033[0m\n", colors[baker->color], baker->name);
        printf("%s%s: PRETZELS are done!  %d/5 recipes are complete!\033[0m\n", colors[baker->color], baker->name, baker->cookies_baked + 
            baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked);
    }

    if (!baker->rolls_baked && baker->yeast && baker->sugar && baker->salt && baker->flour && baker->butter && baker->egg && baker->cinnamon) {
        printf("%s%s: I have all of the ingredients for CINNAMON ROLLS! Starting CINNAMON ROLLS now! \033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.bowl);
        sem_wait(&kitchen.spoon);
        sem_wait(&kitchen.mixer);

        printf("%s%s: I have all the eqipment for CINNAMON ROLLS! Mixing now!\033[0m\n", colors[baker->color], baker->name);

        sem_post(&kitchen.mixer);
        sem_post(&kitchen.spoon);
        sem_post(&kitchen.bowl);

        printf("%s%s: I've made the CINNAMON ROLLS dough! Ready to bake!\033[0m\n", colors[baker->color], baker->name);

        sem_wait(&kitchen.oven);
        printf("%s%s: The CINNAMON ROLLS are in the oven!\033[0m\n", colors[baker->color], baker->name);
        sem_post(&kitchen.oven);
        
        baker->sugar = 0;
        baker->salt = 0;
        baker->flour = 0;
        baker->butter = 0;
        baker->egg = 0;
        baker->cinnamon = 0;
        baker->rolls_baked = 1;
           
        printf("%s%s:Roll call...\033[0m\n", colors[baker->color], baker->name);
        printf("%s%s: CINNAMON ROLLS are done!  %d/5 recipes are complete!\033[0m\n", colors[baker->color], baker->name, baker->cookies_baked + 
            baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked);
    }

    
}



void grab_ingredients(Baker* baker, char* recipe){
    if (recipe == "cookies") {
        //check for cookie items
		for (int j = 0; j < cookies_num_items; j++) {
			if (strcmp(fridge_ingred[i], cookies_ingred[j]) == 0 && baker->cookies_ingred_taken[j] == 0) {
                if (baker->milk == 0){
                    sem_wait(&kitchen.fridge);
                    printf("%s is in the fridge\n", baker->name);
                    baker->milk++;
                    printf("%s took milk from fridge\n", baker->name);
                }
                elif (baker->butter == 0){
                    sem_wait(&kitchen.fridge);
                    printf("%s is in the fridge\n", baker->name);
                    baker->buter++;
                    printf("%s took butter from fridge\n", baker->name);
                }
                printf("%s exiting the fridge\n", baker->name);
                sem_post(&kitchen.fridge); 
                // fridge cs ends
                printf("%s exited the fridge\n", baker->name);
            }

            if (strcmp(pantry_ingred[i], cookies_ingred[j]) == 0 && baker->cookies_ingred_taken[j] == 0) {
                if (baker->flour == 0){
                    sem_wait(&kitchen.pantry);
                    printf("%s is in the pantry\n", baker->name);
                    baker->flour++;
                    printf("%s took flour from pantry\n", baker->name);
                }
                elif (baker->sugar == 0){
                    sem_wait(&kitchen.pantry);
                    printf("%s is in the pantry\n", baker->name);
                    baker->sugar++;
                    printf("%s took sugar from pantry\n", baker->name);
                }
                printf("%s exiting the pantry\n", baker->name);
                sem_post(&kitchen.pantry); 
                // fridge cs ends
                printf("%s exited the pantry\n", baker->name);
            }

            if (baker->milk + baker->flour + baker->butter + baker->sugar == 4){
                baker->cookies_ingred_taken[j] = 1;
            }

        }
	}
	
    
    if (recipe == "pancakes") {
        //check for pancake items
		for (int j = 0; j < pancakes_num_items; j++) {
			if (strcmp(fridge_ingred[i], pancake_ingred[j]) == 0 && baker->pancake_ingred_taken[j] == 0) {
                if (baker->milk == 0){
                    sem_wait(&kitchen.fridge);
                    printf("%s is in the fridge\n", baker->name);
                    baker->milk++;
                    printf("%s took milk from fridge\n", baker->name);
                }
                elif (baker->butter == 0){
                    sem_wait(&kitchen.fridge);
                    printf("%s is in the fridge\n", baker->name);
                    baker->buter++;
                    printf("%s took butter from fridge\n", baker->name);
                }
                printf("%s exiting the fridge\n", baker->name);
                sem_post(&kitchen.fridge); 
                // fridge cs ends
                printf("%s exited the fridge\n", baker->name);
            }

            if (strcmp(pantry_ingred[i], pancake_ingred[j]) == 0 && baker->pancake_ingred_taken[j] == 0) {
                if (baker->flour == 0){
                    sem_wait(&kitchen.pantry);
                    printf("%s is in the pantry\n", baker->name);
                    baker->flour++;
                    printf("%s took flour from pantry\n", baker->name);
                }
                elif (baker->sugar == 0){
                    sem_wait(&kitchen.pantry);
                    printf("%s is in the pantry\n", baker->name);
                    baker->sugar++;
                    printf("%s took sugar from pantry\n", baker->name);
                }
                printf("%s exiting the pantry\n", baker->name);
                sem_post(&kitchen.pantry); 
                // fridge cs ends
                printf("%s exited the pantry\n", baker->name);
            }

            if (baker->milk + baker->flour + baker->butter + baker->sugar == 4){
                baker->pancake_ingred_taken[j] = 1;
            }
        }
    }
    if (recipe == "pizza") {

    }
    if (recipe == "pretzels") {

    }
    if (recipe == "rolls") {

    }
}


    
	





void* thread_callback(void *arg) {
	//char* thread_name = (char*)arg;
	Baker* baker = (Baker*)arg; 

    printf("%s%s: The baking competition is beginning!\033[0m\n", colors[baker->color], baker->name); 




	// fridge cs begins
	sem_wait(&kitchen.fridge);
	printf("%s is in the fridge\n", baker->name);

	//check if baker has items for recipes
    const char* cookies_ingred[] = {"flour", "sugar", "milk", "butter"};
    const int cookies_num_items = 4;
    const char* pancakes_ingred[] = {"flour", "sugar", "baking soda", "salt", "egg", "milk", "butter"};
    const int pancakes_num_items = 7;
    const char* pizza_ingred[] = {"yeast", "sugar", "salt"};
    const int pizza_num_items = 3;
    const char* pretzels_ingred[] = {"flour", "sugar", "salt", "yeast", "baking soda", "egg"};
    const int pretzels_num_items = 6;
    const char* rolls_ingred[] = {"flour", "sugar", "salt", "butter", "eggs", "cinnamon"};


    while (baker->cookies_baked + baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked != 5){
        char* recipe_order[] = {}; //GENERATE A RANDOM ORDER OF RECIPES
        // Ex: "cookies, pancakes, pretzels, pizza, rolls"

        if recipe_order[baker->cookies_baked + baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked] == "cookies"{
            
        }
        elif recipe_order[baker->cookies_baked + baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked] == "pancakes"{

        }
        elif recipe_order[baker->cookies_baked + baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked] == "pizza"{
            
        }
        elif recipe_order[baker->cookies_baked + baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked] == "rolls"{

        }
        elif recipe_order[baker->cookies_baked + baker->pancakes_baked + baker->pizza_baked + baker->pretzels_baked + baker->rolls_baked] == "pretzels"{

        }

    }

	

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

    sem_init(&kitchen.mixer, 0, 2);  //last input value is how many of that resource there is
    sem_init(&kitchen.pantry, 0, 1);
    sem_init(&kitchen.fridge, 0, 2);
    sem_init(&kitchen.bowl, 0, 3);
    sem_init(&kitchen.spoon, 0, 5);
    sem_init(&kitchen.oven, 0, 1);

    sem_init(&kitchen.flour, 0, 1);
    sem_init(&kitchen.sugar,0, 1);
    sem_init(&kitchen.yeast, 0, 1);
    sem_init(&kitchen.baking_soda, 0, 1);
    sem_init(&kitchen.salt,0, 1);
    sem_init(&kitchen.cinnamon,0, 1);

    sem_init(&kitchen.egg, 0, 1);
    sem_init(&kitchen.milk, 0, 1);
    sem_init(&kitchen.butter,0, 1);

	//create n threads and bakers(chatGPT helped)
	for (int i = 0; i < n; i++) {
		snprintf(bakers[i].name, sizeof(bakers[i].name), "baker%d", i + 1);
		memset(bakers[i].cookies_ingred_taken, 0, sizeof(bakers[i].cookies_ingred_taken));
		memset(bakers[i].pancakes_ingred_taken, 0, sizeof(bakers[i].pancakes_ingred_taken));
		
		if (pthread_create(&threads[i], NULL, thread_callback, &bakers[i]) != 0) {
			perror("pthread_create");
			// join previously created threads, cleanup
			for (int k = 0; k < i; k++) pthread_join(threads[k], NULL);
			sem_destroy(&kitchen.fridge);
			sem_destroy(&kitchen.pantry);
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

	sem_destroy(&kitchen.fridge);
	sem_destroy(&kitchen.pantry);
    sem_destroy(&kitchen.mixer);
    sem_destroy(&kitchen.pantry);
    sem_destroy(&kitchen.fridge);
    sem_destroy(&kitchen.bowl);
    sem_destroy(&kitchen.spoon);
    sem_destroy(&kitchen.oven);
    sem_destroy(&kitchen.flour);
    sem_destroy(&kitchen.sugar);
    sem_destroy(&kitchen.yeast);
    sem_destroy(&kitchen.baking_soda);
    sem_destroy(&kitchen.salt);
    sem_destroy(&kitchen.cinnamon);
    sem_destroy(&kitchen.egg);
    sem_destroy(&kitchen.milk);
    sem_destroy(&kitchen.butter);

	return 0;
}