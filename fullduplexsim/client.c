#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converting IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connecting to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Fork
    pid_t pid = fork();

    if (pid == 0) { // Child process
        printf("Child process in client: PID=%d, Parent PID=%d\n", getpid(), getppid());

        while (1) {
            // Send message to server
            memset(buffer, 0, sizeof(buffer));
            printf("Enter message: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strlen(buffer) - 1] = '\0'; // Remove newline
            send(sock, buffer, strlen(buffer), 0);

            if (strcmp(buffer, "BYE") == 0) {
                printf("Exiting.\n");
                break;
            }

            // Receive reply from server
            memset(buffer, 0, sizeof(buffer));
            recv(sock, buffer, sizeof(buffer), 0);
            printf("Received from server: %s\n", buffer);
        }

        exit(0);
    } else if (pid > 0) { // Parent process
        wait(NULL); // Wait for child to finish
        printf("Parent process in client: PID=%d\n", getpid());
    } else {
        perror("Fork failed");
    }

    close(sock);
    return 0;
}

