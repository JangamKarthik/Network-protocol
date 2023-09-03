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
        printf("socket creation error");
        exit(0);
    }
    struct sockaddr_in c_addr;
    c_addr.sin_port = htons(myport);
    c_addr.sin_family = AF_INET;
    c_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(c_addr.sin_zero, '\0', sizeof(c_addr.sin_zero));

    int size = sizeof(struct sockaddr);
    connect(sockfd, (struct sockaddr *)&c_addr, sizeof(c_addr));

    char str1[100], str2[100];

    printf("Enter the first string: ");
    scanf("%s", str1);
    send(sockfd, str1, strlen(str1), 0);

    printf("Enter the second string: ");
    scanf("%s", str2);
    send(sockfd, str2, strlen(str2), 0);

    char response[100];
    recv(sockfd, response, sizeof(response), 0);
    printf("Server Response: %s\n", response);

    close(sockfd);
    return 0;
}