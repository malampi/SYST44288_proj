/* Filename: prod_cons.c
   Authors: Michael Alampi & Eddy Crowder 
   Description: Producer–Consumer Problem using Pthreads */

/* import libraries */
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* define TRUE and FALSE */
#define TRUE 1
#define FALSE 0

/* define mutex lock */
pthread_mutex_t mutex;

/* define semaphores */
sem_t full, empty;

/* buffer counter */
int counter;

/* define buffer */
buffer_item buffer[BUFFER_SIZE];

pthread_t tid;
pthread_attr_t attr;


int insert_item(buffer_item item) {
    /* check if buffer is full */
    if (counter < BUFFER_SIZE) {
        buffer[counter] = item;
        counter++;
        return 0;
    } else {
        /* buffer is full */
        return -1;
    }
}

int remove_item(buffer_item *item) {
    /* check if buffer is not empty */
    if (counter > 0) {
        *item = buffer[(counter-1)];
        counter--;
        return 0;
    }
    /* buffer is empty */
    else {
        return -1;
    }
}

/* initialize variables */
void init() {

    /* create mutex lock */
    pthread_mutex_init(&mutex, NULL);

    /* create full semaphore - init to 0 */
    sem_init(&full, 0, 0);

    /* create empty semaphore - init to BUFFER_SIZE */
    sem_init(&empty, 0, BUFFER_SIZE);

    /* set default attributes */
    pthread_attr_init(&attr);

    /* set buffer counter to 0 */
    counter = 0;
}

void *producer(void *param) {
    buffer_item item;
    while (TRUE) {
        /* compute random number */
        int s_time=rand() / 100000000;

        /* sleep for random number */
        sleep(s_time);

        /* assign random number to item */
        item=rand();
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        /* call insert_item and error-check */
        if (insert_item(item)) {
            printf("%s","error inserting item\n");
        }
        else {
            printf("producer produced %d\n",item);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param) {
    buffer_item item;
    while (TRUE) {
        /* compute random number */
        int s_time=rand() / 100000000;

        /* sleep for random int */
        sleep(s_time);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        /* call remove_item and error-check */
        if (remove_item(&item)) {
            printf("%s","error removing item\n");
        }
        else {
            printf("consumer consumed %d\n", item);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

 int main (int argc, char *argv[])
{
    int i;
    /* validate arg length */
    if (argc < 4) {
        printf("usage: main <INT> <INT> <INT>\n");
        exit(-1);
    }

    /* assign args to variables */
    int sleep_time=atoi(argv[1]);
    int prodNum=atoi(argv[2]);
    int consNum=atoi(argv[3]);
    
    /* initialize variables */
    init();

    /* create producer thread(s) */
    for (int i=0; i<prodNum; i++) {
        pthread_create(&tid,&attr,producer,NULL);
    }

    /* create consumer threads */
    for (int i=0; i<consNum; i++) { 
        pthread_create(&tid,&attr,consumer, NULL);
    }
    
    /* sleep */
    sleep(sleep_time);

    /* exit */
    exit(0);
}