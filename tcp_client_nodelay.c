#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>
#include <netinet/tcp.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void rcvTCP(int sockfd, char *buffer, char *message, int size, int i) {
    int n;
    memset(buffer, 0, sizeof(buffer));
    memset(message, 0, sizeof(message));
    while (strlen(message) < size) {
        n = read(sockfd, buffer, sizeof(buffer));
        if (n < 0) 
             error("ERROR reading from socket");

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
    printf("R: %cx%d %d\n",message[0], strlen(message), i);
    memset(message, 0, sizeof(message));
    if (strlen(buffer) > 0) {
        strncpy(message, buffer, strlen(buffer));
        memset(buffer, 0, sizeof(buffer));
    }
}

void timeSet(unsigned long *times, struct timeval *startTime, struct timeval *finishTime, int i) {
    // finish time
    gettimeofday(finishTime, NULL);
    printf("Start time: %lu.%lu\nFinish time: %lu.%lu\n", startTime->tv_sec, startTime->tv_usec, finishTime->tv_sec, finishTime->tv_usec);
    unsigned long totalSec = finishTime->tv_sec - startTime->tv_sec;
    unsigned long usec;
    if (finishTime->tv_usec > startTime->tv_usec) {
        usec = finishTime->tv_usec - startTime->tv_usec;
    }
    else {
        usec = 1000000 - startTime->tv_usec + finishTime->tv_usec;
    }
    unsigned long total = totalSec * 1000000 + usec;

    // store time
    times[i] = total;

    // start time
    gettimeofday(startTime, NULL);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[64000];
    char message[64000];
    memset(buffer, 0, sizeof(buffer));
    memset(message, 0, sizeof(message));
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    // setup socket
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

    int intarg = 1;
    socklen_t intarg_len = sizeof(intarg);
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &intarg, sizeof(intarg)) < 0)
        printf("Error setsockopt\n");

    if (getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &intarg, &intarg_len) < 0)
        printf("Error getsockopt\n");
    printf("After set and get: intarg = %d length = %d\n", intarg, intarg_len);

    // connect
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // start time
    struct timeval startTime, finishTime;
    unsigned long times[6];
    gettimeofday(&startTime, NULL);

    int i;
    for (i = 1; i < 101; i++) {
        memset(message,'A',1024);
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
            error("ERROR writing to socket");
        printf("S: %cx%d %d   ",message[0], strlen(message), i);
        rcvTCP(sockfd, buffer, message, 1024, i);
    }

    timeSet(times, &startTime, &finishTime, 0);

    for (i = 1; i < 101; i++) {
        memset(message,'B',4096);
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
            error("ERROR writing to socket");
        printf("S: %cx%d %d   ",message[0], strlen(message), i);
        rcvTCP(sockfd, buffer, message, 4096, i);
    }

    timeSet(times, &startTime, &finishTime, 1);

    for (i = 1; i < 101; i++) {
        memset(message,'C',8192);
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
            error("ERROR writing to socket");
        printf("S: %cx%d %d   ",message[0], strlen(message), i);
        rcvTCP(sockfd, buffer, message, 8192, i);
    }

    timeSet(times, &startTime, &finishTime, 2);

    for (i = 1; i < 101; i++) {
        memset(message,'D',16384);
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
            error("ERROR writing to socket");
        printf("S: %cx%d %d   ",message[0], strlen(message), i);
        rcvTCP(sockfd, buffer, message, 16384, i);
    }

    timeSet(times, &startTime, &finishTime, 3);

    for (i = 1; i < 101; i++) {
        memset(message,'E',32768);
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
            error("ERROR writing to socket");
        printf("S: %cx%d %d   ",message[0], strlen(message), i);
        rcvTCP(sockfd, buffer, message, 32768, i);
    }

    timeSet(times, &startTime, &finishTime, 4);

    for (i = 1; i < 101; i++) {
        memset(message, 'F', sizeof(message));
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
            error("ERROR writing to socket");
        printf("S: %cx%d %d   ",message[0], strlen(message), i);
        rcvTCP(sockfd, buffer, message, 64000, i);
    }

    timeSet(times, &startTime, &finishTime, 5);

    memset(message, 0, sizeof(message));
    sprintf(message, "done");
    n = write(sockfd,message,strlen(message));
    if (n < 0) 
         error("ERROR writing to socket");
    memset(buffer, 0, sizeof(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0)
        error("ERROR reading from socket");
    printf("Last receive: %s\n", buffer);

    int size;
    printf("\nTime elapsed for each run:\n");
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
        printf("%6d bytes - Total: %8luus  Average: %6luus\n", size, times[i], times[i]/100);
    }
    close(sockfd);
    return 0;
}
