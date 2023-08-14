#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == 0) {
        printf("Child process in server: PID=%d, Parent PID=%d\n", getpid(), getppid());
        send(new_socket, "hello", sizeof("hello"), 0);
        exit(0);
    } else if (pid > 0) {
        wait(NULL);
        printf("Parent process in server: PID=%d\n", getpid());

        memset(buffer, 0, sizeof(buffer));
        valread = read(new_socket, buffer, sizeof(buffer));
        printf("Received from client: %s\n", buffer);

        send(new_socket, "hey", sizeof("hey"), 0);
    } else {
        perror("Fork failed");
    }

    close(new_socket);
    return 0;
}

