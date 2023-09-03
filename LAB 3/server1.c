#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>

#define myport 1234
#define MAX_BUFFER_SIZE 256

void handleClient(int clientSocket) {
    int jyo;
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        printf("Child PID: %d, PPID: %d\n", getpid(), getppid());

        char buffer[MAX_BUFFER_SIZE];
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                break;  // Connection closed by the client
            }

            printf("Client: %s", buffer);

            // Send a response to the client
            printf("Server: ");
            fgets(buffer, sizeof(buffer), stdin);
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        close(clientSocket);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        wait(jyo);
        printf("Parent PID: %d, Child PID: %d\n", getpid(), child_pid);
    }
}

int main() {
    int size;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Socket creation
    struct sockaddr_in server_addr, c_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(sockfd, 10);
    printf("The server is ready for listening.\n");
    size = sizeof(struct sockaddr);

    while (1) {
        int afd = accept(sockfd, (struct sockaddr *)&c_addr, &size);
        printf("A new client connected.\n");

        handleClient(afd);
    }

    close(sockfd);

    return 0;
}
