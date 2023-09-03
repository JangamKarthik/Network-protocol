#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define myport 1234

struct Student {
    char regno[15];
    char name[15];
    char add[10];
    char dep[10];
    char sem[1];
    char section[1]; 
    char c_reg[3];
    char marks[3];  
};

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

    struct Student stu[2];

    strcpy(stu[0].regno, "210911388");
    strcpy(stu[1].regno, "210911128");
    strcpy(stu[0].name, "KARTHIK");
    strcpy(stu[1].name, "JOHN PORK");
    strcpy(stu[0].c_reg[0], "ict123");
    strcpy(stu[0].c_reg[1], "ict223");
    strcpy(stu[0].c_reg[2], "ict456");
    strcpy(stu[1].c_reg[0], "ict827");
    strcpy(stu[1].c_reg[1], "ict345");
    strcpy(stu[1].c_reg[2], "ict123");
    strcpy(stu[0].marks[0] , "89");
    strcpy(stu[0].marks[1] , "88");
    strcpy(stu[0].marks[2] , "87");
    strcpy(stu[1].marks[0] , "86");
    strcpy(stu[1].marks[1] , "85");
    strcpy(stu[1].marks[2] , "84");
    strcpy(stu[0].add, "xyzhdeeh");
    strcpy(stu[1].add, "ijnciwni");
    strcpy(stu[0].dep, "ICT");
    strcpy(stu[1].dep, "ICT");
    strcpy(stu[0].sem, "5");
    strcpy(stu[1].sem, "5");
    strcpy(stu[0].section, "B");
    strcpy(stu[0].section, "A"); 
    

    char PID[5];    
    char buffer[100];

    recv(afd, buffer, sizeof(buffer), 0);
    if (strcmp(buffer, stu[0].regno) == 0 || strcmp(buffer, stu[1].regno) == 0) {
        int pid = fork();
        if (pid == 0) {
            if (strcmp(stu[0].regno, buffer) == 0) {
                strcpy(buffer, " ");
                strcpy(buffer, "Name of student is: ");
                strcat(buffer, stu[0].name);
                strcat(buffer, " Adress is: ");
                strcat(buffer, stu[0].add);
                strcat(buffer, " PID is: ");
                strcat(buffer, PID);
				goto sender;
            } else if (strcmp(stu[1].regno, buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Name of student is: ");
                strcat(buffer, stu[1].name);
                strcat(buffer, " Adress is: ");
                strcat(buffer, stu[1].add);
                strcat(buffer, " PID is: ");
                strcat(buffer, PID);
				goto sender;
            } else {
                strcpy(buffer, "");
                strcpy(buffer, "INCORRECT INPUT\n");
            }
         sender:   send(afd, buffer, sizeof(buffer), 0);
        }
		
    } else if (strcmp(buffer, stu[0].name) == 0 || strcmp(buffer, stu[1].name) == 0) {
        int p = fork();
        if (p == 0) {
            if (strcmp(stu[0].name, buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Dept sem courses registered are: ");
                strcat(buffer, stu[0].dep);
                strcat(buffer, " ");
                strcat(buffer, PID);
                strcat(buffer, " ");
                strcat(buffer, stu[0].c_reg[0]);
                strcat(buffer, " ");
                strcat(buffer, stu[0].c_reg[1]);
                strcat(buffer, " ");
                strcat(buffer, stu[0].c_reg[2]);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else if (strcmp(stu[1].name, buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Dept sem courses registered are: ");
                strcat(buffer, stu[1].dep);
                strcat(buffer, " ");
                strcat(buffer, PID);
                strcat(buffer, " ");
                strcat(buffer, stu[1].c_reg[0]);
                strcat(buffer, " ");
                strcat(buffer, stu[1].c_reg[1]);
                strcat(buffer, " ");
                strcat(buffer, stu[1].c_reg[2]);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else {
                strcpy(buffer, "");
                strcpy(buffer, "INCORRECT INPUT\n");
            }
         sender: send(afd, buffer, sizeof(buffer), 0);
        }
    } else if (strcmp(stu[0].c_reg[0], buffer) == 0 || strcmp(stu[0].c_reg[1], buffer) == 0 || strcmp(stu[0].c_reg[2], buffer) == 0 ||
               strcmp(stu[1].c_reg[0], buffer) == 0 || strcmp(stu[1].c_reg[1], buffer) == 0 || strcmp(stu[1].c_reg[2], buffer) == 0) {
        int pd = fork();
        if (pd == 0) {
            if (strcmp(stu[0].c_reg[0], buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Marks are: ");
                strcat(buffer, PID);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else if (strcmp(stu[0].c_reg[1], buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Marks are: ");
                strcat(buffer, PID);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else if (strcmp(stu[0].c_reg[2], buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Marks are: ");
                strcat(buffer, PID);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else if (strcmp(stu[1].c_reg[0], buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Marks are: ");
                strcat(buffer, PID);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else if (strcmp(stu[1].c_reg[1], buffer) == 0) {
                strcpy(buffer, "");
                strcpy(buffer, "Marks are: ");
                strcat(buffer, PID);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else if (strcmp(stu[1].c_reg[2], buffer) == 0) {
                strcpy(buffer, "");
                sprintf(PID, "%d", stu[1].marks[2]);
                strcat(buffer, PID);
                strcat(buffer, "PID is: ");
                strcat(buffer, PID);
                goto sender;
            } else {
                strcpy(buffer, "");
                strcpy(buffer, "INCORRECT INPUT\n");
            }

         sender: send(afd, buffer, sizeof(buffer), 0);
        }
    } else {
        strcpy(buffer, "");
        strcpy(buffer, "INCORRECT INPUT\n");
        send(afd, buffer, sizeof(buffer), 0);
    }

    close(sockfd);
    close(afd);
}