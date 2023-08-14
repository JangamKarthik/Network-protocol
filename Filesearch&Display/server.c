#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receiving file name from client
    memset(buffer, 0, sizeof(buffer));
    read(new_socket, buffer, sizeof(buffer));
    printf("Received file name: %s\n", buffer);

    // Opening file
    FILE *file = fopen(buffer, "r");
    if (file == NULL) {
        send(new_socket, "File not present", sizeof("File not present"), 0);
        close(new_socket);
        exit(EXIT_SUCCESS);
    }

    // Reading and displaying file contents
    printf("File contents:\n");
    char c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    fclose(file);
    close(new_socket);

    return 0;
}

