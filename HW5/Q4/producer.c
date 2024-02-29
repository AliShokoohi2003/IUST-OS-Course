#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

#define SHARED_MEMORY_NAME "/shared_memory"
#define STACK_SIZE 100

typedef struct {
    int stack[STACK_SIZE];
    int count;
    int tp;
    sem_t empty;
    sem_t full;
    sem_t mutex;
} SharedMemory;

void produce(SharedMemory *sharedMemory) {

    int num = rand() % 20 + 1;
    sem_wait(&sharedMemory->empty);
    sem_wait(&sharedMemory->mutex);
    printf("Produced: %d\n", num);
    sharedMemory->stack[sharedMemory->tp] = num;
    sharedMemory->tp++;
    sharedMemory->count++;
    sem_post(&sharedMemory->mutex);
    sem_post(&sharedMemory->full);
}

int main() {
    srand(time(NULL));
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedMemory));
    SharedMemory *sharedMemory = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sharedMemory->tp = 0;
    sharedMemory->count = 0;
    sem_init(&sharedMemory->empty, 1, STACK_SIZE);
    sem_init(&sharedMemory->full, 1, 0);
    sem_init(&sharedMemory->mutex, 1, 1);

    while (1) {
        produce(sharedMemory);
        sleep(1);
    }

    munmap(sharedMemory, sizeof(SharedMemory));
    shm_unlink(SHARED_MEMORY_NAME);

    return 0;
}
