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

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    int size = sizeof(struct sockaddr);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Connection error\n");
        close(sockfd);
        exit(0);
    }

    printf("Socket connected.\n");
    char buffer[100];
        printf("Enter a valid detail to fetch: \n");
        scanf("%s",buffer);
        send(sockfd,buffer,sizeof(buffer),0);
        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("\n%s\n", buffer);

    close(sockfd);
    return 0;
}
