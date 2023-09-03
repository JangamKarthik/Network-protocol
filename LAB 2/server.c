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

void searchAndSendCount(FILE *file, int afd, char *searchString) {
    char line[100];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, searchString) != NULL) {
            count++;
        }
    }
    if (count > 0) {
        char response[50];
        sprintf(response, "The string was found %d times.", count);
        send(afd, response, sizeof(response), 0);
    } else {
        char response[50] = "String not found.";
        send(afd, response, sizeof(response), 0);
    }
}

void replaceString(FILE *file, int afd, char *str1, char *str2) {
    FILE *tempFile = fopen("tempfile.txt", "w");
    char line[100];
    int replaced = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *pos;
        while ((pos = strstr(line, str1)) != NULL) {
            replaced = 1;
            int index = pos - line;
            strncpy(pos, str2, strlen(str2));
            strcpy(pos + strlen(str2), line + index + strlen(str1));
        }
        fprintf(tempFile, "%s", line);
    }
    fclose(file);
    fclose(tempFile);
    if (replaced) {
        remove("original.txt");
        rename("tempfile.txt", "original.txt");
        char response[50] = "String replaced.";
        send(afd, response, sizeof(response), 0);
    } else {
        char response[50] = "String not found.";
        send(afd, response, sizeof(response), 0);
    }
}

void reorderFile(FILE *file) {
    char lines[100][100];
    char line[100];
    int numLines = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[numLines], line);
        numLines++;
    }
    qsort(lines, numLines, sizeof(lines[0]), (int (*)(const void *, const void *))strcmp);
    fclose(file);
    file = fopen("original.txt", "w");
    for (int i = 0; i < numLines; i++) {
        fprintf(file, "%s", lines[i]);
    }
    fclose(file);
}

int main() {
    int size;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //socket creation
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
    printf("the server is listening\n");

    char buffer[50];
    char filename[50];
    int choice;

    recv(afd, buffer, sizeof(buffer), 0);
    strcpy(filename, buffer);

    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        strcpy(buffer, "The file exists");
        printf("The file exists\n");
        send(afd, buffer, sizeof(buffer), 0);
    } else {
        strcpy(buffer, "the file does not exist !");
        send(afd, buffer, sizeof(buffer), 0);
        printf("file does not exist !\n");
        fclose(file);
        close(afd);
        close(sockfd);
        exit(0);
    }

    while (1) {
        recv(afd, &choice, sizeof(choice), 0);
        switch (choice) {
            case 1:
                recv(afd, buffer, sizeof(buffer), 0);
                int size;
                recv(afd, &size, sizeof(size), 0);
                searchAndSendCount(file, afd, buffer);
                fclose(file);
                file = fopen(filename, "r");
                break;

            case 2:
                char buffer1[50], buffer2[50];
                recv(afd, buffer1, sizeof(buffer1), 0);
                recv(afd, buffer2, sizeof(buffer2), 0);
                replaceString(file, afd, buffer1, buffer2);
                fclose(file);
                file = fopen(filename, "r");
                break;

            case 3:
                printf("Arranging in ascending order of ASCII");
						fflush(stdout);
                        char filecontent[500];
                        int count=fread(&filecontent,sizeof(char),500,file);
						char tempArr[500];
						strcpy(tempArr, filecontent);
						for(int i=0; i<count-1; i++){
							for(int j=0; j<count-i-1; j++){
								if(tempArr[j]>tempArr[j+1]){
									char temp = tempArr[j];
									tempArr[j] = tempArr[j+1];
									tempArr[j+1] = temp;
								}
							}
						}
						send(afd, tempArr, sizeof(tempArr), 0);
						break;
            case 4:
                close(sockfd);
                exit(0);
        }
    }
}