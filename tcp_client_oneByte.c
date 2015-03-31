#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>

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

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    
    //start timer
    struct timeval startTime, finishTime;
    gettimeofday(&startTime, NULL);
    
    //loop 1000 times
    int i;
    for (i = 0; i < 1000; i++) {
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("%s\n",buffer);
    }
    
    // finish time
    gettimeofday(&finishTime, NULL);
    printf("Start time: %lu.%d\nFinish time: %lu.%d\n", startTime.tv_sec, startTime.tv_usec, finishTime.tv_sec, finishTime.tv_usec);
    unsigned long totalSec = finishTime.tv_sec - startTime.tv_sec;
    unsigned long usec;
    if (finishTime.tv_usec > startTime.tv_usec) {
        usec = finishTime.tv_usec - startTime.tv_usec;
    }
    else {
        usec = 1000000 - startTime.tv_usec + finishTime.tv_usec;
    }
    unsigned long total = totalSec * 1000000 + usec;
    printf("Total latency: %lu\nAverage latency: %lu\n", total, total/1000);
    
    close(sockfd);
    return 0;
}














