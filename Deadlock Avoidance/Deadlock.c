/*
//  Deadlock.c
//  CST-221
//
//  Created by David Mundt on 12/1/20.
 
    This is the resource I used for the logger:
        http://simplestcodings.blogspot.com/2010/10/simple-logger-in-c.html
 
    I didn't just copy paste all this code to my project. I typed every line individually and
        implemented my file. After about 4 hours of testing, I could not end up getting the
        log to print to the file, I have no idea how to get it to work, and I wouldn't even
        know where to begin to create my own. I feel like I have already spent too much time
        on this part, and I feel like I am no behind on the rest of this assignment which
        will easily take up 10+ hours of my time, most of that time spent teaching myself how
        to code the solution. I also have no idea how to implement it into the assignment
        solution;
 
    As for the rest of the code, I found bits and pieces of it online and tried to make it
        fit. I am just going to let you know that I really do not have much of a clue as to
        what is exactly happening because I don't know C. I tried to document everythinig I
        did understand though.
 
    I have easily spent over 30 hours on this. Wish there were a better way to learn this
        because I have lost my motivation for this class. Haven't even started the research
        project yet.
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

FILE *fp;
static int SESSION_TRACKER; //Keeps track of session

char* print_time() {
    int size = 0;
    time_t t;
    char *buf;
    
    t = time(NULL); // Gets the current calendar time
    
    char *timestr = asctime(localtime(&t));
    timestr[strlen(timestr) - 1] = 0; // Getting rid of \n
    
    size = strlen(timestr) + 1 + 2; //Additional +2 for square braces
    buf = (char*)malloc(size);
    
    memset(buf, 0x0, size);
    snprintf(buf, size, "[%s]", timestr);
    
    return buf;
}

void log_print(char* filename, int line, char *fmt, ...) {
    va_list list;
    char *p, *r;
    int e;
    
    if(SESSION_TRACKER > 0) {
        fp = fopen("File.txt", "a+");
    }
    else {
        fp = fopen("File.txt", "w");
    }
    
    fprintf(fp, "%s", print_time());
    fprintf(fp, "[%s][line: %d] ", filename, line);
    va_start(list, fmt);
    
    for(p = fmt; *p; ++p) {
        if(*p != '%') { // If simple string
            fputc(*p, fp);
        }
        else {
            switch(*p++) {
                // String
                case 's': {
                    r = va_arg(list, char*);
                    
                    fprintf(fp, "%s", r);
                    continue;
                }
                
                // Integer
                case 'd': {
                    e = va_arg(list, int);
                    
                    fprintf(fp, "%d", e);
                    continue;
                }
                    
                default:
                    fputc(*p, fp);
            }
        }
    }
    va_end(list);
    fputc('\n', fp);
    SESSION_TRACKER++;
    fclose(fp);
}

#define PHILOSOPHERS 5
#define DELAY 5000
#define FOOD 50

void *philosopher (void *id);
void grab_utensil(int, int, char *);
void down_utensil(int, int);

pthread_mutex_t utensil[PHILOSOPHERS]; // Tried using fork but it is a special word
pthread_t philo[PHILOSOPHERS];
pthread_mutex_t food_lock;
int sleep_seconds = 0;

// Function that takes food off of the table when it has been eaten, and returns the remaining
int food_on_table() {
    static int food = FOOD;
    int myfood;
    
    pthread_mutex_lock(&food_lock);
    if(food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock(&food_lock);
    return myfood;
}

int main (int argn, char **argv) {
    int i;
    
    if(argn == 2) {
        sleep_seconds = atoi(argv[1]); // Converts a string arg to an int
    }
    
    // Thread for keeping track of food
    pthread_mutex_init(&food_lock, NULL);
    
    // Initilizes the defined number of philosophers
    for(i = 0; i < PHILOSOPHERS; i++) {
        pthread_mutex_init(&utensil[i], NULL);
    }
    
    // Creates the defined number of philosophers
    for(i = 0; i < PHILOSOPHERS; i++) {
        pthread_create(&philo[i], NULL, philosopher, (void *)i);
    }
    
    // Joins the defined number of philosophers
    for(i = 0; i < PHILOSOPHERS; i++) {
        pthread_join(philo[i], NULL);
    }
    
    return 0;
}

// Philosopher function/object
void *philosopher(void *num) {
    int id;
    int left_utensil, right_utensil, f;
    
    id = (int)num; // Cast void to int
    printf("Philosopher %d is done thinking and now ready to eat. \n", id);
    right_utensil = id;
    left_utensil = id + 1;
    
    // Theres not a utensil with an id < the number of philosophers
    if(left_utensil == PHILOSOPHERS) {
        left_utensil = 0;
    }
    
    f = food_on_table();
    while(f) {
        if (id == 1) {
            sleep(sleep_seconds);
        }
        
        grab_utensil(id, right_utensil, "right ");
        grab_utensil(id, left_utensil, "left ");
        
        printf("Philosopher %d: eating.\n", id);
        usleep(DELAY * (FOOD - f + 1)); // Determins how long the philosopher eats depending
                                        // on how much food is left.
        down_utensil(left_utensil, right_utensil);
    }
    
    printf("Philosopher %d is done eating.\n", id);
    return (NULL);
}

// Function for grabbing the utensils from the table, or locking them
void grab_utensil (int phil, int c, char *hand) {
    pthread_mutex_lock(&utensil[c]);
    printf("Philosopher %d: got %s utensil %d\n", phil, hand, c);
}

// Function for returning the utensils to the table or unlocking them
void down_utensil(int u1, int u2) { // Ints are for utensil ids
    pthread_mutex_unlock(&utensil[u1]);
    pthread_mutex_unlock(&utensil[u2]);
}
