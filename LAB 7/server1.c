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

// Caesar Cipher encryption function
void caesarEncrypt(char *str, int key) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            str[i] = (str[i] + key - 'a') % 26 + 'a';
        }
    }
}

int main() {
    int size;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket creation
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

    char usernames[5][50] = {"user1", "user2", "user3", "user4", "user5"};
    char passwords[5][50] = {"pass1", "pass2", "pass3", "pass4", "pass5"};
    double balances[5] = {1000.0, 2000.0, 3000.0, 4000.0, 5000.0};
    char buffer[50];

    while (1) {
        recv(afd, buffer, sizeof(buffer), 0);
        char received_username[50], received_password[50];
        sscanf(buffer, "%s %s", received_username, received_password);

        // Verify username
        int i;
        int valid_user = 0;
        for (i = 0; i < 5; i++) {
            if (strcmp(usernames[i], received_username) == 0) {
                valid_user = 1;
                break;
            }
        }

        if (!valid_user) {
            send(afd, "Incorrect Username", sizeof("Incorrect Username"), 0);
        } else {
            // Caesar Cipher encryption for password
            caesarEncrypt(received_password, 3);

            if (strcmp(passwords[i], received_password) != 0) {
                send(afd, "Incorrect Password", sizeof("Incorrect Password"), 0);
            } else {
                // Successful login
                send(afd, "Login Successful", sizeof("Login Successful"), 0);

                while (1) {
                    recv(afd, buffer, sizeof(buffer), 0);
                    if (buffer[0] == 'd') { // Debit
                        double amount;
                        sscanf(buffer + 2, "%lf", &amount);
                        balances[i] -= amount;
                    } else if (buffer[0] == 'b') { // Credit
                        double amount;
                        sscanf(buffer + 2, "%lf", &amount);
                        balances[i] += amount;
                    } else if (buffer[0] == 'c') { // View Balance
                        sprintf(buffer, "Your balance: %.2lf", balances[i]);
                        send(afd, buffer, sizeof(buffer), 0);
                    } else if (buffer[0] == 'e') { // EXIT
                        break;
                    }
                }
                close(afd); // Moved the close() here to close the connection after the client exits
                break; // Exit the loop after the client exits
            }
        }
    }
    close(sockfd);

}
