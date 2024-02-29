#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define RESOURCES_NUM 5
#define THREADS_NUM 10

typedef struct {
    int resources[RESOURCES_NUM];
    sem_t availableResources;
    pthread_mutex_t poolMutex;
} ResourceManager;

ResourceManager resourceManager;

void* threadFunction(void* arg) {
    int threadId = *((int*)arg);
    int tasks = rand() % 5 + 1;
    for (int i = 0; i < tasks; i++){

        sem_wait(&resourceManager.availableResources);
        pthread_mutex_lock(&resourceManager.poolMutex);
        int resourceId = -1;
        for (int j = 0; j < RESOURCES_NUM; j++) {
            if (resourceManager.resources[j] < 0) {
                resourceId = j;
                resourceManager.resources[j] = threadId;
                break;
            }
        }

        pthread_mutex_unlock(&resourceManager.poolMutex);
        printf("Thread %d is performing work with resource %d\n", threadId, resourceId);
        sleep(2.5); 

        pthread_mutex_lock(&resourceManager.poolMutex);
        resourceManager.resources[resourceId] = -1;
        pthread_mutex_unlock(&resourceManager.poolMutex);

        sem_post(&resourceManager.availableResources);
    }
    return NULL;
}

int main() {
    pthread_t threads[THREADS_NUM];
    int threadIds[THREADS_NUM];

    for (int i = 0; i < RESOURCES_NUM; i++) {
        resourceManager.resources[i] = -1;
    }
    sem_init(&resourceManager.availableResources, 0, RESOURCES_NUM);
    pthread_mutex_init(&resourceManager.poolMutex, NULL);

    for (int i = 0; i < THREADS_NUM; i++) {
        threadIds[i] = i + 1;
        pthread_create(&threads[i], NULL, threadFunction, &threadIds[i]);
    }

    for (int i = 0; i < THREADS_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&resourceManager.availableResources);
    pthread_mutex_destroy(&resourceManager.poolMutex);

    return 0;
}
