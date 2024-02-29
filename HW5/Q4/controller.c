#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>

#define SHARED_MEMORY_NAME "/shared_memory"
#define STACK_SIZE 100
#define HIGH_THRESHOLD 20
#define LOW_THRESHOLD 10

typedef struct {
    int stack[STACK_SIZE];
    int count;
    int tp;
    sem_t empty;
    sem_t full;
    sem_t mutex;
} SharedMemory;

void controlConsumers(SharedMemory *sharedMemory,int p_count) {
    while (1) {
        sem_wait(&sharedMemory->mutex);

        int freeSpace = STACK_SIZE - sharedMemory->tp;
        if (sharedMemory->count > HIGH_THRESHOLD) {
            p_count++;
            pid_t pid = fork();

            if (pid == 0) {
                char *args[] = {"./consumer", NULL};
                execvp(args[0], args);

            }
        }
         else if (sharedMemory->count < LOW_THRESHOLD) {
            if (p_count > 0)
            {
            system("pkill -f ./consumer");
            p_count--;
            }
        }
        
        sem_post(&sharedMemory->mutex);
    }
}
    

int main() 
{
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);
    SharedMemory *sharedMemory = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int p_count = 0;
    controlConsumers(sharedMemory,p_count);

    munmap(sharedMemory, sizeof(SharedMemory));

    return 0;
}
