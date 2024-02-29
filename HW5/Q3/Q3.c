#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h> 

#define MAX 15

void initMatrix(int rows, int cols, int matrix[MAX][MAX]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void displayMatrix(int rows, int cols, int matrix[MAX][MAX]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    #define M_ROWS 3
    #define N 3
    #define K_COLS 3

    key_t keys[] = {IPC_PRIVATE, IPC_PRIVATE, IPC_PRIVATE};
    int shmid[] = {shmget(keys[0], sizeof(int) * M_ROWS * N, IPC_CREAT | 0666),
                   shmget(keys[1], sizeof(int) * N * K_COLS, IPC_CREAT | 0666),
                   shmget(keys[2], sizeof(int) * M_ROWS * K_COLS, IPC_CREAT | 0666)};

    int (*matrix1)[MAX] = shmat(shmid[0], NULL, 0);
    int (*matrix2)[MAX] = shmat(shmid[1], NULL, 0);
    int (*result)[MAX] = shmat(shmid[2], NULL, 0);

    initMatrix(M_ROWS, N, matrix1);
    initMatrix(N, K_COLS, matrix2);

    printf("Matrix M1:\n");
    displayMatrix(M_ROWS, N, matrix1);

    printf("\nMatrix M2:\n");
    displayMatrix(N, K_COLS, matrix2);

    for (int i = 0; i < M_ROWS; ++i) {
        if (fork() == 0) {
            for (int j = 0; j < K_COLS; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                    
                }
                printf("M3[%d][%d] = %d\n", i, j, result[i][j]);
                printf("\nChild process %d completed.\n", getpid());
            }
            exit(0);
        }
    }

    for (int i = 0; i < M_ROWS; ++i) {
        wait(NULL);
    }

    printf("\nMatrix M3:\n");
    displayMatrix(M_ROWS, K_COLS, result);

    shmdt(matrix1);
    shmdt(matrix2);
    shmdt(result);
    shmctl(shmid[0], IPC_RMID, NULL);
    shmctl(shmid[1], IPC_RMID, NULL);
    shmctl(shmid[2], IPC_RMID, NULL);

    return 0;
}
