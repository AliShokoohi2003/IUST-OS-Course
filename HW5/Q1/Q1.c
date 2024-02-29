#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    int source[2], mean_pipe[2], mean_pipe_2[2], min_pipe[2], min_pipe_2[2];
    if (pipe(source) == -1 || pipe(mean_pipe) == -1 || pipe(mean_pipe_2) == -1 || pipe(min_pipe) == -1 || pipe(min_pipe_2) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) { 
        FILE *file = fopen("input.txt", "r");
        int totalNumbers;
        fscanf(file, "%d", &totalNumbers);
        write(source[1], &totalNumbers, sizeof(int));
        write(source[1], &totalNumbers, sizeof(int));
        double numbers[totalNumbers];
        for (int i = 0; i < totalNumbers; i++) {
            fscanf(file, "%lf", &numbers[i]);
        }
        fclose(file);
        double mean;
        double min;  
        write(min_pipe[1], numbers, totalNumbers * sizeof(double));
        write(mean_pipe[1], numbers, totalNumbers * sizeof(double));

        
        read(mean_pipe_2[0], &mean,sizeof(double));
        read(min_pipe_2[0], &min,sizeof(double));
        printf("Mean = %.2f, ",mean); 
        printf("Min = %.2f\n", min);
        
        exit(EXIT_SUCCESS);
    }
    pid_t pid2 = fork();

    if (pid2 == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0) {
        int totalNumbers;
        read(source[0], &totalNumbers, sizeof(int));
        double numbers[totalNumbers];
        read(mean_pipe[0], numbers, totalNumbers * sizeof(double));
        double total = 0;
        for (int i = 0; i < totalNumbers; i++) {
            total += numbers[i];
        }
        double mean = total/totalNumbers;
        write(mean_pipe_2[1], &mean, sizeof(double));
        exit(EXIT_SUCCESS);
    }
        
    wait(NULL);
    pid_t pid3 = fork();

    if (pid3 == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    if (pid3 == 0) { 
        int totalNumbers;
        read(source[0], &totalNumbers,sizeof(int));
        double numbers[totalNumbers];
        read(min_pipe[0], numbers, totalNumbers * sizeof(double));
        
        double min = numbers[0];
        for (int i = 0; i < totalNumbers; i++) {
            if (numbers[i] < min) {
                min = numbers[i];
            }
        }   
        write(min_pipe_2[1],&min,sizeof(double));
        exit(EXIT_SUCCESS);
    }    
    return 0;
}
