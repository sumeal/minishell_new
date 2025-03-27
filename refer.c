#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_CMDS 10

void run_pipeline(char *commands[], int num_commands) {
    int pipefds[2 * (num_commands - 1)];

    // Create all pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipefds + i * 2) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Fork and execute commands
    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) { // Child process
            // Input redirection (for commands after the first)
            if (i > 0) {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1) {
                    perror("dup2 input");
                    exit(1);
                }
            }

            // Output redirection (for commands before the last)
            if (i < num_commands - 1) {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1) {
                    perror("dup2 output");
                    exit(1);
                }
            }

            // Close all pipe file descriptors in the child process
            for (int j = 0; j < 2 * (num_commands - 1); j++) {
                close(pipefds[j]);
            }

            // Execute the command (e.g., `command[i]`)
            execlp(commands[i], commands[i], (char *)NULL);
            // If exec fails
            perror("exec");
            exit(1);
        }
    }

    // Parent process: Close all pipe file descriptors
    for (int i = 0; i < 2 * (num_commands - 1); i++) {
        close(pipefds[i]);
    }

    // Wait for all child processes
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}

int main() {
    // Example: Run "ls | grep 'test' | wc -l"
    char *commands[] = { "ls", "grep", "wc" };
    int num_commands = 3;

    run_pipeline(commands, num_commands);

    return 0;
}
