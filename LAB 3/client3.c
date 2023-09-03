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
        printf("Socket creation error\n");
        exit(0);
    }

    struct sockaddr_in c_addr;
    c_addr.sin_port = htons(myport);
    c_addr.sin_family = AF_INET;
    c_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(c_addr.sin_zero, '\0', sizeof(c_addr.sin_zero));

    int size = sizeof(struct sockaddr);
    connect(sockfd, (struct sockaddr *)&c_addr, sizeof(c_addr));

    char input[256];
    char output[256];

    printf("Enter an alphanumeric string: ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0'; 

    send(sockfd, input, strlen(input), 0);

    recv(sockfd, output, sizeof(output), 0);
    printf("Received sorted results from server: %s\n", output);

    close(sockfd);
    return 0;
}