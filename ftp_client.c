#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_TCP_PORT 5035
#define MAX 60

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char send[MAX], recvline[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    printf("\nEnter the source file name : \n");
    scanf("%s", send);

    if (write(sockfd, send, MAX) == -1) {
        perror("Write error");
        close(sockfd);
        exit(1);
    }

    while ((n = read(sockfd, recvline, MAX)) > 0) {
        write(STDOUT_FILENO, recvline, n);
    }

    if (n < 0) {
        perror("Read error");
    }

    close(sockfd);
    return 0;
}