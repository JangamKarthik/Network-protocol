// Q1 TCP program to send an array of numbers to the server and choose from 4 options to do the required stuff
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

    int choice;
    char buffer[100];

    printf("Enter the file name: ");
    scanf("%s", buffer);
    send(sockfd, buffer, sizeof(buffer), 0);
    printf("File name sent\n");

    recv(sockfd, buffer, 100, 0);

    if (strcmp(buffer, "The file exists") == 0) {
        printf("The file exists\n");
    } else {
        printf("File does not exist!\n");
        close(sockfd);
        exit(0);
    }

    while (1) {
        printf("\n\n1. Search for a string");
        printf("\n2. Replace a string");
        printf("\n3. Reorder your string");
        printf("\n4. Exit");
                printf("\nEnter your choice: ");
        scanf("%d", &choice);
        send(sockfd, &choice, sizeof(choice), 0);

        switch (choice) {
            case 1:
                printf("Enter the size of the string: ");
                int size;
                scanf("%d", &size);
                printf("Enter the string to be searched: ");
                scanf("%s", buffer);
                send(sockfd, buffer, sizeof(buffer), 0);
                send(sockfd, &size, sizeof(size), 0);

                recv(sockfd, buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);

                int count;
                recv(sockfd, &count, sizeof(count), 0);
                printf("The number of occurrences: %d\n", count);
                break;

            case 2:
                char buffer1[50], buffer2[50];
                printf("Enter the string to be replaced: ");
                scanf("%s", buffer1);
                printf("Enter the string to replace with: ");
                scanf("%s", buffer2);
                send(sockfd, buffer1, sizeof(buffer1), 0);
                send(sockfd, buffer2, sizeof(buffer2), 0);

                recv(sockfd, buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);
                break;

            case 3:
                recv(sockfd, buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);

                // Receive and display the reordered file content from the server
                recv(sockfd, buffer, sizeof(buffer), 0);
                printf("Reordered File Content:\n");
                printf("%s\n", buffer);
                break;

            case 4:
                close(sockfd);
                exit(0);
        }
    }
}