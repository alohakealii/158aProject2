#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void rcvTCPandSend(int sockfd, char *buffer, char *message, int size, int i) {
    int n;
    while (strlen(message) < size) {
        n = read(sockfd, buffer, sizeof(buffer));
        if (n < 0) 
             error("ERROR reading from socket");
        if (n == 0)
            printf("read == 0\n");

        // if there are more bytes than fit for the current message
        if (strlen(message) + strlen(buffer) > size) {
            int amount = size - strlen(message); // calculate bytes for current message
            int j;

            // copy remaining byes for current message
            for (j = 0; j < amount; j++) {
                message[strlen(message)] = buffer[j];
            }

            // zero the rest of the buffer, leaving the excess belonging to next message
            memset(buffer + amount, 0, sizeof(buffer) - amount);
        }
        else {
            strcat(message, buffer);
            memset(buffer, 0, sizeof(buffer));
        }
    }
    printf("R: %cx%d %d   ",message[0], strlen(message), i);
    n = write(sockfd, message, strlen(message));
    if (n < 0)
        error("Error writing to socket");
    printf("S: %cx%d %d\n", message[0], strlen(message), i);
    memset(message, 0, sizeof(message));
    if (strlen(buffer) > 0) {
        strncpy(message, buffer, strlen(buffer));
        memset(buffer, 0, sizeof(buffer));
    }
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[64000];
    char message[64000];
    memset(message, 0, sizeof(message));
    memset(buffer, 0, sizeof(buffer));
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

    // set TCP maximum segment size
    int intarg = 460;
    socklen_t intarg_len = sizeof(intarg);
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &intarg, sizeof(intarg)) < 0)
        printf("Error setsockopt\n");

    if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &intarg, &intarg_len) < 0)
        printf("Error getsockopt\n");
    printf("After set and get: intarg = %d  len = %d\n", intarg, sizeof(intarg_len));
     

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
            rcvTCPandSend(newsockfd, buffer, message, size, j);
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