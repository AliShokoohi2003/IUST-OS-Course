#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

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

void consume(SharedMemory *sharedMemory) {
    sem_wait(&sharedMemory->full);
    sem_wait(&sharedMemory->mutex);
    int num = sharedMemory->stack[sharedMemory->tp - 1];
    printf("Consumed: %d\n", num);
    sharedMemory->tp--;
    sharedMemory->count--;
    sem_post(&sharedMemory->mutex);
    sem_post(&sharedMemory->empty);
    sleep(2);
}

int main() {
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);
    SharedMemory *sharedMemory = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    while (1) {
        consume(sharedMemory);
    }

    munmap(sharedMemory, sizeof(SharedMemory));

    return 0;
}
