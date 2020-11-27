/*
    ProducerConsumer.c
    CST-221

    Created by David Mundt on 11/26/20.
    
    My solution here was to create 2 threads, one for 'producer' and one for 'consumer'.
        I also created a very simple buffer in order to access the information I needed for
        each function. I made sure that the 'producer' is always the first one to run, and I
        gave enough time for the producer to create the product before the consuemer could
        consume more than what was being produced.
*/

#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define N 100 // How big the buffer size will be
int count = 0; // Number of items in the buffer
int lower = 0; // Next slot to read in the buffer
int upper = 0; // Next slot to write in the buffer
int buffer[N];

// Puts the product in the buffer
void put(int i) {
    buffer[upper - 1] = i;
}

// Gets the product from the buffer
int get() {
    lower = lower + 1;
    return buffer[lower - 1];
}

// Adds a product to the buffer
int produce() {
    upper = upper + 1;
    return upper - 1;
}

// Prints the product
void consume(int i) {
    printf("%i\n", i);
}

// Produces the product
void *producer(void *value) {
    printf("producer\n");
    
    int i;
    while(1) {
        if(count == N) {
            sleep(5); // Gives time for the consumer to consume
            return NULL;
        }
        i = produce();
        put(i);
        count = count + 1;
    }
}

// Consumes the product
void *consumer(void *value) {
    printf("consumer\n");
    
    int i;
    while(1) {
        if(count == 0) {
            sleep(5); // Makes sure the producer is the first to process
        }
        if(lower == upper) {
            sleep(5); // Gives time for the producer to make enough for the consumer to consume
        }
        i = get();
        consume(i);
        if(i == N - 1) { // Ends the consuemr thread if the consumer has consumed all products
            return NULL;
        }
    }
}

int main() {
    pthread_t producerThread; // Thread for the producer
    pthread_t consumerThread; //Thread for the consumer
    
    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);
    
    pthread_join(producerThread, NULL);
    pthread_join(producerThread, NULL);
    
    return 1;
}
