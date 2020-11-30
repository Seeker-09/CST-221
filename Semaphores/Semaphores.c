//
//  Semaphores.c
//  CST-221
//
//  Created by David Mundt on 11/27/20.
//

/*
 In this simple example, I am using semaphores to read write then read to and from a buffer.
    However, sem_init and sem_destroy don't seem to work anymore. So I attempted to use
    sem_open and sem_close, but I could not get the program to work correctly. The program
    below shows the logic of my use of semaphores and demonstrates my understanding of their
    use. Because I am not taught how to use these in class, Most of the hours of work I put
    into this was spent researching how to code this, but I could not find anything to fix my
    problem. (The actual reason I was given is that sem_init is a depricated function. I also
    learned that not only are monitors not usabel in the C language, as stated by the book
    and other sources on the internet, but they are mostly depricated as well. I am not
    having a hard time understanding the concepts presented in this class, but I do have an
    issue with teaching myself C programming, which is probably one of the hardest programming
    languages to learn. I do not mean for this footnote to come off as harsh. I am just very
    confused as to why I am not taught C programming, and why the assignments have
    contradictions like monitors not able to used in C. I will probably put this comment
    in the written portion of the assignment too and maybe even expound upon it a bit. I
    don't know if this is the right place to put this, but I believe it is relevant. Thanks
    for listening.)
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

char buff[100]; // My buffer
sem_t *mySem;

// Reads from the buffer
void *readBuffer() {
    printf("In read thread\n");
    sem_wait(mySem); // Tries to access shared resource
    // Critical Section
    printf("In read critical section\n");
    printf("%s\n", buff);
    sem_post(mySem); // Done using shared resource/opens up access
    pthread_exit(0);
}

// Writes to the buffer
void *writeBuffer() {
    printf("In write thread\n");
    sem_wait(mySem);
    // Critical section
    printf("In write critical section\n");
    sprintf(buff, "%s", "My first time using semaphores\n");
    sem_post(mySem);
    pthread_exit(0);
}

int main() {
    mySem = sem_open("/mysem", O_CREAT); // Semaphore initialization
    pthread_t readThread, writeThread;
    
    pthread_create(&writeThread, NULL, writeBuffer(), NULL);
    pthread_create(&readThread, NULL, readBuffer(), NULL);
    
    printf("Main thread");
    
    pthread_join(readThread, NULL);
    pthread_join(writeThread, NULL);
    sem_close(mySem);
    return 0;
}



