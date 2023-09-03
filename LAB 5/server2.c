#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define myport 1234

int areAnagrams(char *str1, char *str2) {
    int count1[26] = {0};
    int count2[26] = {0};

    int i;

    for (i = 0; str1[i] != '\0'; i++) {
        count1[str1[i] - 'a']++;
    }

    for (i = 0; str2[i] != '\0'; i++) {
        count2[str2[i] - 'a']++;
    }

    for (i = 0; i < 26; i++) {
        if (count1[i] != count2[i]) {
            return 0; 
        }
    }

    return 1;
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

    while (1) {
        size = sizeof(struct sockaddr);
        int afd = accept(sockfd, (struct sockaddr *)&c_addr, &size);
        printf("Connected to client at %s:%d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

        char str1[100], str2[100];
        char response[100];

        
        recv(afd, str1, sizeof(str1), 0);
        recv(afd, str2, sizeof(str2), 0);

        if (areAnagrams(str1, str2)) {
            strcpy(response, "Anagrams");
        } else {
            strcpy(response, "Not Anagrams");
        }

        time_t now;
        struct tm *local_time;
        time(&now);
        local_time = localtime(&now);
        char datetime[100];
        strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", local_time);

        strcat(response, " (");
        strcat(response, datetime);
        strcat(response, ")");

        send(afd, response, strlen(response), 0);

        close(afd);
    }

    close(sockfd);
    return 0;
}