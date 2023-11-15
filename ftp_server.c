#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_TCP_PORT 5035
#define MAX 60

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, clength;
    struct sockaddr_in serv_addr, cli_addr;
    char str[MAX];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    printf("\nBinding...");
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("Binding error");
        close(sockfd);
        exit(1);
    }

    printf("\nListening...");
    listen(sockfd, 5);
    clength = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clength);
    close(sockfd);

    if (newsockfd == -1) {
        perror("Accepting connection failed");
        exit(1);
    }

    read(newsockfd, str, MAX);
    printf("\nClient message\n File Name: %s\n", str);

    FILE *f1 = fopen(str, "r");
    if (f1 == NULL) {
        perror("File opening failed");
        close(newsockfd);
        exit(1);
    }

    char buff[MAX];
    while (fgets(buff, MAX, f1) != NULL) {
        write(newsockfd, buff, strlen(buff)); // Use strlen to send only the actual content
        printf("\n");
    }

    fclose(f1);
    printf("\nFile Transferred\n");

    close(newsockfd);
    return 0;
}