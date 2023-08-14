#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Fork
    pid_t pid = fork();

    if (pid == 0) { // Child process
        printf("Child process in server: PID=%d, Parent PID=%d\n", getpid(), getppid());

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            recv(new_socket, buffer, sizeof(buffer), 0);
            printf("Received from client: %s\n", buffer);

            if (strcmp(buffer, "BYE") == 0) {
                printf("Client said BYE. Exiting.\n");
                break;
            }

            // Send reply to client
            send(new_socket, "hey", sizeof("hey"), 0);
        }

        exit(0);
    } else if (pid > 0) { // Parent process
        wait(NULL); // Wait for child to finish
        printf("Parent process in server: PID=%d\n", getpid());
    } else {
        perror("Fork failed");
    }

    close(new_socket);
    return 0;
}

