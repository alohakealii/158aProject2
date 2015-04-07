#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    int sockfd, port;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)  {
        fprintf(stderr,"ERROR opening socket\n");
        exit(1);
    }
    port = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr,"ERROR on binding\n");
        exit(1);
    } 

    socklen_t clilen = sizeof(cli_addr);

    char message;
    n = recvfrom(sockfd, &message, 1, 0, (struct sockaddr *)&cli_addr, &clilen);
    if (n < 0) {
        fprintf(stderr,"ERROR reading from socket\n");
        exit(1);
    }
    printf("Here is the message: %c\n", message);

    n = sendto(sockfd, &message, 1, 0, (struct sockaddr *)&cli_addr, clilen);
    if (n < 0) {
        fprintf(stderr,"ERROR writing to socket\n");
        exit(1);
    }
    close(sockfd);
    return 0; 
}
