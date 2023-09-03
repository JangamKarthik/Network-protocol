#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#define myport 1234

void sortNumbers(char *input) {
    int len = strlen(input);
    int numbers[len];
    int numCount = 0;

    for (int i = 0; i < len; i++) {
        if (isdigit(input[i])) {
            numbers[numCount++] = input[i] - '0';
            input[i] = ' '; 
        }
    }

    
    for (int i = 0; i < numCount - 1; i++) {
        for (int j = 0; j < numCount - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    
    int idx = 0;
    for (int i = 0; i < len; i++) {
        if (input[i] == ' ' && idx < numCount) {
            input[i] = '0' + numbers[idx++];
        }
    }
    char out[100];
    int k=0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i]-'0'>=0&&input[i]-'0'<=9)
        {
            out[k++] = input[i];
        }
        
    }
    out[k] = '\0';
    strcpy(input, out);
    
}

void sortCharacters(char *input) {
    int len = strlen(input);

    
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (input[i] < input[j]) {
                char temp = input[i];
                input[i] = input[j];
                input[j] = temp;
            }
        }
    }

    
    int newLen = 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(input[i])) {
            input[newLen++] = input[i];
        }
    }
    input[newLen] = '\0'; 
}

int main() {
    int size;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    struct sockaddr_in server_addr, c_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(sockfd, 10);
    printf("The server is ready for listening.\n");
    size = sizeof(struct sockaddr);

    int afd = accept(sockfd, (struct sockaddr *)&c_addr, &size);
    printf("The server is listening\n");

    char input[256];
    char output[256];

    recv(afd, input, sizeof(input), 0);
    printf("Received string from client: %s\n", input);

    pid_t child_pid = fork();

    if (child_pid == 0) {
        sortNumbers(input);
        printf("Output at the child process of the server: %s\n", input);
        send(afd, input, strlen(input), 0);
        printf("Child process ID: %d\n", getpid());
        goto end;
    } else {
        for (int i = 0; i < strlen(input); i++) {
            output[i] = input[i];
        }
        output[strlen(input)] = '\0';
        sortCharacters(output);
        printf("Output at the parent process of the server: %s\n", output);
        send(afd, output, strlen(output), 0);
        printf("Parent process ID: %d\n", getpid());
    }

end: close(afd);
     return 0;
}