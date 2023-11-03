#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    printf("Connected to the server.\n");

    char filename[100];
    char buffer[1000];

    printf("Enter the filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove the newline character

    int file_fd = open(filename, O_RDONLY);
    if (file_fd == -1) {
        printf("File not found.\n");
        close(sockfd);
        exit(1);
    }

    while (1) {
        int n = read(file_fd, buffer, sizeof(buffer));
        if (n == 0) {
            break; // End of file
        }

        send(sockfd, buffer, n, 0);
    }

    close(file_fd);
    close(sockfd);
    printf("File sent to the server.\n");

    // Now receive and display the results from the server
    printf("Contents received from the server:\n");
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            break; // End of file or error
        }
        printf("%s", buffer);
    }

    return 0;
}
