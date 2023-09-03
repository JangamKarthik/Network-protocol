#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define myport 1234
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error");
        exit(0);
    }
    struct sockaddr_in c_addr;
    c_addr.sin_port = htons(myport);
    c_addr.sin_family = AF_INET;
    c_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(c_addr.sin_zero, '\0', sizeof(c_addr.sin_zero));

    int size = sizeof(struct sockaddr);
    connect(sockfd, (struct sockaddr *)&c_addr, sizeof(c_addr));

    while (1) {
        char query[MAX_BUFFER_SIZE];
        printf("Enter a domain name (or 'exit' to quit): ");
        fgets(query, sizeof(query), stdin);

        query[strcspn(query, "\n")] = '\0';

        if (strcmp(query, "exit") == 0) {
            break;
        }

        int sntb = send(sockfd, query, strlen(query), 0);
        if (sntb == -1) {
            close(sockfd);
            printf("Message Sending Failed\n");
            exit(0);
        }

        char response[MAX_BUFFER_SIZE];
        int recb = recv(sockfd, response, sizeof(response), 0);
        if (recb == -1) {
            close(sockfd);
            printf("Message Receiving Failed\n");
            exit(0);
        }

        response[recb] = '\0';
        printf("IP Address: %s\n", response);
    }

    close(sockfd);
    return 0;
}