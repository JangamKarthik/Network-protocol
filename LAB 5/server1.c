#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define myport 1234
#define MAX_CLIENTS 2

int main() {
    int size;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket creation
    struct sockaddr_in server_addr, c_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(sockfd, 10);
    printf("The server is ready for listening.\n");

    int clients[MAX_CLIENTS];
    int connected_clients = 0;

    while (connected_clients < MAX_CLIENTS) {
        size = sizeof(struct sockaddr);
        clients[connected_clients] = accept(sockfd, (struct sockaddr *)&c_addr, &size);
        printf("Client %d connected.\n", connected_clients + 1);

        char message[100];
        sprintf(message, "Client %d connected from %s:%d\n", connected_clients + 1, inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        send(clients[connected_clients], message, strlen(message), 0);

        connected_clients++;
    }

    if (connected_clients == MAX_CLIENTS) {
        printf("Two clients connected. Combining strings...\n");
        FILE *file = fopen("data.txt", "a");
        fprintf(file, "Manipal ");
        char buffer[100];

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int recb = recv(clients[i], buffer, sizeof(buffer), 0);
            if (recb == -1) {
                printf("Message Receiving Failed from Client %d\n", i + 1);
                continue;
            }
            buffer[recb] = '\0';
            fprintf(file, "%s ", buffer);
        }

        fclose(file);

        printf("Manipal Institute of Technology\n");

        for (int i = 0; i < MAX_CLIENTS; i++) {
            char response[] = "Server: Session Terminated\n";
            send(clients[i], response, strlen(response), 0);
            close(clients[i]);
        }

        close(sockfd);
    }

    return 0;
}
