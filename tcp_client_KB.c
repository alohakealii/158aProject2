#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[64000];
    memset(buffer,0,64000);
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    int i;
    for (i = 0; i < 100; i++) {
        memset(buffer,'A',1024);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        printf("S: %cx%d %d   ",buffer[0], strlen(buffer), i);
        memset(buffer,0,1024);
        n = read(sockfd,buffer,1024);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("R: %cx%d %d\n",buffer[0], strlen(buffer), i);
    }
    for (i = 0; i < 100; i++) {
        memset(buffer,'B',4096);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        printf("S: %cx%d %d   ",buffer[0], strlen(buffer), i);
        memset(buffer,0,4096);
        n = read(sockfd,buffer,4096);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("R: %cx%d %d\n",buffer[0], strlen(buffer), i);
    }
    for (i = 0; i < 100; i++) {
        memset(buffer,'C',8192);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        printf("S: %cx%d %d   ",buffer[0], strlen(buffer), i);
        memset(buffer,0,8192);
        n = read(sockfd,buffer,8192);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("R: %cx%d %d\n",buffer[0], strlen(buffer), i);
    }
    for (i = 0; i < 100; i++) {
        memset(buffer,'D',16384);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
         printf("S: %cx%d %d   ",buffer[0], strlen(buffer), i);
        memset(buffer,0,16384);
        n = read(sockfd,buffer,16384);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("R: %cx%d %d\n",buffer[0], strlen(buffer), i);
    }
    for (i = 0; i < 100; i++) {
        memset(buffer,'E',32768);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
         printf("S: %cx%d %d   ",buffer[0], strlen(buffer), i);
        memset(buffer,0,32768);
        n = read(sockfd,buffer,32768);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("R: %cx%d %d\n",buffer[0], strlen(buffer), i);
    }
    for (i = 0; i < 100; i++) {
        memset(buffer, 'F', sizeof(buffer));
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        printf("S: %cx%d %d   ",buffer[0], strlen(buffer), i);
        memset(buffer, 0, sizeof(buffer));
        n = read(sockfd,buffer, sizeof(buffer));
        if (n < 0) 
             error("ERROR reading from socket");
        printf("R: %cx%d %d\n", buffer[0], strlen(buffer), i);
    }
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "done");
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    memset(buffer, 0, sizeof(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0)
        error("ERROR reading from socket");
    printf("Last receive: %s\n", buffer);
    close(sockfd);
    return 0;
}