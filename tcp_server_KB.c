#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void rcvTCP(int sockfd, char *buffer, char *message, int size, int i) {
    memset(message, 0, sizeof(message));
    int n;
    while (strlen(message) < size) {
        memset(buffer, 0, sizeof(buffer));
        n = read(sockfd,buffer,1024);
        if (n < 0) 
             error("ERROR reading from socket");
        strcat(message, buffer);
        printf("strlen(message)=%d  size=%d\n", strlen(message), size);
    }
    printf("R: %cx%d %d   ",message[0], strlen(message), i);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[64000];
    char message[64000];
    struct sockaddr_in serv_addr, cli_addr;
    clilen = sizeof(cli_addr);
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    // setup socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // bind
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    // listen
    listen(sockfd,10);

    // accept
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");

    int i;
    int n;
    int size = 0;
    for (i = 0; i < 6; i++) {
        memset(message, 0, sizeof(message));
        memset(buffer, 0, sizeof(buffer));
        if (i == 0)
            size = 1 * 1024;
        else if (i == 1)
            size = 4 * 1024;
        else if (i == 2)
            size = 8 * 1024;
        else if (i == 3)
            size = 16 * 1024;
        else if (i == 4)
            size = 32 * 1024;
        else
            size = 64000;
        int j;
        for (j = 1; j < 101; j++) {
            rcvTCP(newsockfd, buffer, message, size, j);
            n = write(newsockfd, message, strlen(message));
            if (n < 0)
                error("Error writing to socket");
            printf("S: %cx%d %d\n", message[0], strlen(message), j);
        }      
    }

    memset(buffer, 0, sizeof(buffer));
    n = read(newsockfd, buffer, sizeof(buffer));
    if (n < 0)
        error("Error reading from socket");
    printf("Last receive: %s\n", buffer);
    n = write(newsockfd, buffer, strlen(buffer));
    if (n < 0)
        error("Error writing to socket");

    close(newsockfd);
    close(sockfd);
    return 0; 
}