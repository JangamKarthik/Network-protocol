#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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

    char username[50], password[50], choice[2], amount[20], buffer[100];

    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);
    
    // Caesar Cipher encryption for password
    for (int i = 0; password[i] != '\0'; i++) {
        if (isalpha(password[i])) {
            password[i] = (password[i] - 3 - 'a' + 26) % 26 + 'a';
        }
    }

    // Send username and password to the server
    sprintf(buffer, "%s %s", username, password);
    send(sockfd, buffer, sizeof(buffer), 0);

    recv(sockfd, buffer, sizeof(buffer), 0);
    if (strcmp(buffer, "Incorrect Username") == 0 || strcmp(buffer, "Incorrect Password") == 0) {
        printf("%s\n", buffer);
        close(sockfd);
        exit(0);
    } else {
        printf("Logged in successfully.\n");

        while (1) {
            recv(sockfd, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);

            printf("Enter your choice: ");
            scanf("%s", choice);

            if (choice[0] == 'd' || choice[0] == 'b') {
                printf("Enter the amount: ");
                scanf("%s", amount);
                sprintf(buffer, "%s %s", choice, amount);
                send(sockfd, buffer, sizeof(buffer), 0);
            } else if (choice[0] == 'c') {
                send(sockfd, choice, sizeof(choice), 0);
                recv(sockfd, buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);
            } else if (choice[0] == 'd') {
                send(sockfd, choice, sizeof(choice), 0);
                break;
            }
        }
    }
    close(sockfd);
    return 0;
}
