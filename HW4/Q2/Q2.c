#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore_1;
sem_t semaphore_2;

void* p1(void* arg){
    sem_wait(&semaphore_1);

    printf("F ");
    
    sem_post(&semaphore_2);

    printf("E ");
    printf("G ");
}

void* p2(void* arg){
    printf("A ");

    sem_post(&semaphore_1);
    sem_wait(&semaphore_2);

    printf("C ");
    printf("B ");
}

int main(){
    sem_init(&semaphore_1, 0, 0);
    sem_init(&semaphore_2, 0, 0);

    pthread_t thread_1;
    pthread_t thread_2;
    pthread_create(&thread_1, NULL, p1, NULL);
    pthread_create(&thread_2, NULL, p2, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    sem_destroy(&semaphore_1);
    sem_destroy(&semaphore_2);

    return 0;
}