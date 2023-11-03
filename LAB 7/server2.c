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
    int size;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Socket creation
    struct sockaddr_in server_addr, c_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(sockfd, 10);
    printf("The server is ready for listening.\n");
    size = sizeof(struct sockaddr);

    int afd = accept(sockfd, (struct sockaddr *)&c_addr, &size);
    printf("Connected to a client.\n");

    char buffer[1000];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(afd, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Client has disconnected. Exiting...\n");
            break;
        }

        printf("Client: %s\n", buffer);

        printf("Server (you): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove the newline character
        send(afd, buffer, sizeof(buffer), 0);
    }

    close(afd);
    close(sockfd);
}
