#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define BUFFER_SIZE 240 // Define the size of the buffer used for message passing


int main()
{
    int fd1, fd2; // File descriptors for the named pipes (FIFO)
    char * myfifo1 = "/tmp/myfifo1"; // Path to the first named pipe
    char * myfifo2 = "/tmp/myfifo2"; // Path to the second named pipe
    char buffer[BUFFER_SIZE]; // Buffer to store messages for communication
    pid_t pid; // Variable to store the process ID after fork


    // Create the first named pipe with read-write permissions (0666)
    mkfifo(myfifo1, 0666);

    // Create the second named pipe with read-write permissions (0666)
    mkfifo(myfifo2, 0666);

    // Fork a new process to create a parent-child relationship
    pid = fork();

    // Check if the fork failed
    if (pid == -1) {
        fprintf(stderr, "Fork failed");
        return 1; // Exit the program with an error named_pipesnamed_pipesnamed_pipes
    }

    if (pid == 0) {
        // This block is executed by the child process
        char modified[BUFFER_SIZE]; // Buffer to store the modified message
        ssize_t n;

        // Open the read end of the first pipe
        fd1 = open(myfifo1, O_RDONLY); // Variable to store the number of bytes read

        // Read the message sent by the parent from the first pipe
        n = read(fd1, buffer, BUFFER_SIZE);

        // Process the message: reverse the case of each character
        for (int i = 0; i < n; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                // Convert lowercase to uppercase
                modified[i] = buffer[i] - 32;
            }
            else if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                // Convert uppercase to lowercase
                modified[i] = buffer[i] + 32;
            }
            else {
                // Keep other characters unchanged
                modified[i] = buffer[i];
            }
        }

        // Open the second pipe (myfifo2) in write-only mode
        fd2 = open(myfifo2, O_WRONLY);

        // Send the modified message to the parent through the second pipe
        write(fd2, modified, n);

        // Close both pipe file descriptors
        close(fd1);
        close(fd2);
    }
    else {
        // This block is executed by the parent process
        ssize_t n; // Variable to store the number of bytes read or written


        // Open the first pipe (myfifo1) in write-only mode
        fd1 = open(myfifo1, O_WRONLY);

        // Send a message to the child process through the first pipe
        write(fd1, "My Name is Cyberraf", strlen("My Name is Cyberraf") + 1);

        // Close the first pipe after writing the message
        close(fd1);

        // Open the second pipe (myfifo2) in read-only mode
        fd2 = open(myfifo2, O_RDONLY);

        // Read the modified message from the child process
        n = read(fd2, buffer, BUFFER_SIZE);

        // Print the modified message to the console
        printf("%.*s\n", (int)n, buffer);

        // Close the second pipe after reading the message
        close(fd2);
    }

    // Remove the named pipes to clean up resources
    unlink(myfifo1);
    unlink(myfifo2);

    return 0; // Exit the program successfully
}

