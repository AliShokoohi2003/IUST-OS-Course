#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // File descriptors for pipes
    int pipe1[2], pipe2[2];
    
    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // First command: ls -a
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) { // Child process
        // Close unnecessary pipes
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        // Redirect stdout to pipe1
        dup2(pipe1[1], STDOUT_FILENO);

        // Execute command
        execlp("ls", "ls", "-a", NULL);

        // If execlp fails
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }

    // Second command: grep p
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) { // Child process
        // Close unnecessary pipes
        close(pipe1[1]);
        close(pipe2[0]);

        // Redirect stdin from pipe1 and stdout to pipe2
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        // Execute command
        execlp("grep", "grep", "p", NULL);

        // If execlp fails
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }

    // Third command: wc -l
    pid_t pid3 = fork();
    if (pid3 == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid3 == 0) { // Child process
        // Close unnecessary pipes
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        // Redirect stdin from pipe2
        dup2(pipe2[0], STDIN_FILENO);

        // Execute command
        execlp("wc", "wc", "-l", NULL);

        // If execlp failsx
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }

    // Close all pipes in the parent process
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Wait for all child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}
