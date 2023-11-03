#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define myport 1234

// Function to convert text to uppercase
void convertToUppercase(char *text) {
    for (int i = 0; text[i]; i++) {
        text[i] = toupper(text[i]);
    }
}

// Function to replace letters with their corresponding digits
void replaceWithDigits(char *text) {
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            text[i] = text[i] - 'a' + '1';
        }
    }
}

int main() {
    int size;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Socket creation
    struct sockaddr_in server_addr, c_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(sockfd, 10);
    printf("The server is ready for listening.\n");
    size = sizeof(struct sockaddr);

    int afd = accept(sockfd, (struct sockaddr *)&c_addr, &size);
    printf("Connected to a client.\n");

    char buffer[1000];
    int file_fd;

    int child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child_pid == 0) {
        // Child process: Convert text to uppercase
        memset(buffer, 0, sizeof(buffer));
        int n = recv(afd, buffer, sizeof(buffer), 0);
        convertToUppercase(buffer);

        // Write the result to a file
        file_fd = open("uppercase.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        write(file_fd, buffer, n);
        close(file_fd);
    } else {
        // Parent process: Replace letters with digits
        wait(NULL); // Wait for the child to finish

        file_fd = open("uppercase.txt", O_RDONLY);
        if (file_fd == -1) {
            printf("Error opening uppercase.txt\n");
            close(afd);
            close(sockfd);
            exit(1);
        }

        int result_file_fd = open("result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            int n = read(file_fd, buffer, sizeof(buffer));
            if (n == 0) {
                break; // End of file
            }

            replaceWithDigits(buffer);
            write(result_file_fd, buffer, n);
        }
        close(file_fd);
        close(result_file_fd);

        // Send the result to the client
        result_file_fd = open("result.txt", O_RDONLY);
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            int n = read(result_file_fd, buffer, sizeof(buffer));
            if (n == 0) {
                break; // End of file
            }
            send(afd, buffer, n, 0);
        }
        close(result_file_fd);
    }

    close(afd);
    close(sockfd);
}
