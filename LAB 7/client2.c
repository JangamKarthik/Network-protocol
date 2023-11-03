#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define myport 1234

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation error\n");
        exit(0);
    }
    struct sockaddr_in c_addr;
    c_addr.sin_port = htons(myport);
    c_addr.sin_family = AF_INET;
    c_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP
    memset(c_addr.sin_zero, '\0', sizeof(c_addr.sin_zero));

    int size = sizeof(struct sockaddr);
    connect(sockfd, (struct sockaddr *)&c_addr, sizeof(c_addr));
    printf("Connected to the server.\n");

    char buffer[1000];

    while (1) {
        printf("Client (you): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove the newline character
        send(sockfd, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("You have disconnected. Exiting...\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        recv(sockfd, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Server has disconnected. Exiting...\n");
            break;
        }

        printf("Server: %s\n", buffer);
    }

    close(sockfd);
}
