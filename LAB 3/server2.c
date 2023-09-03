#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#define myport 1324
#define MAX_MSG_SIZE 256

// Function to print all permutations of a string
void permute(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
    } else {
        for (int i = l; i <= r; i++) {
            // Swap characters at positions l and i
            char temp = str[l];
            str[l] = str[i];
            str[i] = temp;

            // Recursively generate permutations for the remaining characters
            permute(str, l + 1, r);

            // Swap back to backtrack and generate other permutations
            temp = str[l];
            str[l] = str[i];
            str[i] = temp;
        }
    }
}

int main() {
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr, client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&(server_addr.sin_zero), '\0', sizeof(server_addr.sin_zero));

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    printf("Server is ready to receive.\n");

    char buffer[MAX_MSG_SIZE];
    socklen_t size = sizeof(struct sockaddr);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &size);

        if (bytesReceived < 0) {
            perror("Error in receiving data");
            continue;
        }

        printf("Received String: %s\n", buffer);
        printf("Permutations:\n");
        permute(buffer, 0, strlen(buffer) - 1);
    }

    close(sockfd);

    return 0;
}
