/***
 Team: Rafik Tarbari, Christos Haramis
 Date: February 23, 2023
 Class: CSC-4210 Spring 2023

 Description:
	This program uses named pipes in which one process sends a
        string message to a second process, and the second process reverses
        the case of each character in the message and sends it back to the first
        process. For example, if the first process sends the message Hi There,
        the second process will return hI tHERE. This will require using two
        pipes, one for sending the original message from the first to the second
        process and the other for sending the modified message from the second
        to the first process. You can write this program using either UNIX or
        Windows pipes

 To Compile: gcc -Wall cf_named_pipes.c -o cf_named_pipes

 To Execute: ./cf_named_pipes

***/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 100

int main()
{
    int fd1, fd2;
    char * myfifo1 = "/tmp/myfifo1";
    char * myfifo2 = "/tmp/myfifo2";
    char buffer[BUFFER_SIZE];
    pid_t pid;

    // Create the named pipes
    mkfifo(myfifo1, 0666);
    mkfifo(myfifo2, 0666);

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid == 0) {
        // This is the child process
        char modified[BUFFER_SIZE];
        ssize_t n;

        // Open the read end of the first pipe
        fd1 = open(myfifo1, O_RDONLY);

        // Read the message from the first pipe
        n = read(fd1, buffer, BUFFER_SIZE);

        // Modify the message by reversing the case of each character
        for (int i = 0; i < n; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                modified[i] = buffer[i] - 32;
            }
            else if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                modified[i] = buffer[i] + 32;
            }
            else {
                modified[i] = buffer[i];
            }
        }

        // Open the write end of the second pipe
        fd2 = open(myfifo2, O_WRONLY);

        // Write the modified message to the second pipe
        write(fd2, modified, n);

        // Close the pipe file descriptors
        close(fd1);
        close(fd2);
    }
    else {
        // This is the parent process
        ssize_t n;

        // Open the write end of the first pipe
        fd1 = open(myfifo1, O_WRONLY);

        // Send a message to the child process through the first pipe
        write(fd1, "Hi There", strlen("Hi There") + 1);

        // Close the write end of the first pipe
        close(fd1);

        // Open the read end of the second pipe
        fd2 = open(myfifo2, O_RDONLY);

        // Read the modified message from the second pipe
        n = read(fd2, buffer, BUFFER_SIZE);

        // Print the modified message to the console
        printf("%.*s\n", (int)n, buffer);

        // Close the pipe file descriptors
        close(fd2);
    }

    // Remove the named pipes
    unlink(myfifo1);
    unlink(myfifo2);

    return 0;
}

