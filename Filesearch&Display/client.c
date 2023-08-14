#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converting IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connecting to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Getting the file name from user
    printf("Enter the file name: ");
    memset(buffer, 0, sizeof(buffer));
    scanf("%s", buffer);
    send(sock, buffer, strlen(buffer), 0);

    // Receiving response from server
    memset(buffer, 0, sizeof(buffer));
    valread = read(sock, buffer, sizeof(buffer));
    if (strcmp(buffer, "File not present") == 0) {
        printf("%s\n", buffer);
        close(sock);
        exit(EXIT_SUCCESS);
    }

    printf("File contents received from server.\n");

    close(sock);
    return 0;
}