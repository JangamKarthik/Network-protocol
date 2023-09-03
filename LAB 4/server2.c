#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define myport 1234
#define MAX_BUFFER_SIZE 1024

struct DNSRecord {
    char domain[50];
    char ip_address[20];
};

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
    printf("The server is listening\n");

    // Read DNS records from the database.txt file
    FILE *db_file = fopen("database.txt", "r");
    if (db_file == NULL) {
        perror("Error opening database.txt");
        exit(1);
    }

    struct DNSRecord records[100];
    int num_records = 0;

    char line[MAX_BUFFER_SIZE];
    while (fgets(line, sizeof(line), db_file) != NULL) {
        sscanf(line, "%s %s", records[num_records].domain, records[num_records].ip_address);
        num_records++;
    }

    fclose(db_file);

    while (1) {
        char client_query[MAX_BUFFER_SIZE];
        int recb = recv(afd, client_query, sizeof(client_query), 0);
        if (recb == -1) {
            printf("\nMessage Receiving Failed");
            close(sockfd);
            close(afd);
            exit(0);
        }

        client_query[recb] = '\0'; // Null-terminate the received message

        // Lookup the IP address for the given domain
        char *response = "Not Found"; // Default response
        for (int i = 0; i < num_records; i++) {
            if (strcmp(client_query, records[i].domain) == 0) {
                response = records[i].ip_address;
                break;
            }
        }

        // Send the response to the client
        int sntb = send(afd, response, strlen(response), 0);
        if (sntb == -1) {
            printf("\nMessage Sending Failed");
            close(sockfd);
            close(afd);
            exit(0);
        }
    }

    close(afd);
    close(sockfd);

    return 0;
}
