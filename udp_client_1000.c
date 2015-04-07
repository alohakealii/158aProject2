#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    int sockfd, port, length;
    struct sockaddr_in serv_addr, my_addr;

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    port = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr,"ERROR opening socket\n");
        exit(0);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(port);

    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    socklen_t servlen = sizeof(serv_addr);

    printf("Please enter the message: ");
    char message = getchar();

    // start time
    struct timeval startTime, finishTime;
    gettimeofday(&startTime, NULL);

    int i;
    for (i = 0; i < 1000; i++) {
        length = sendto(sockfd, &message, 1, 0, (struct sockaddr *)&serv_addr, servlen);
        if (length < 0) {
            fprintf(stderr,"ERROR writing to socket\n");
            exit(0);
        }

        length = recvfrom(sockfd, &message, 1, 0, (struct sockaddr *)&serv_addr, &servlen);
        if (length < 0) {
            fprintf(stderr,"ERROR reading from socket\n");
            exit(0);
        }
        printf("Server response: %c\n",message);
    }

    // finish time
    gettimeofday(&finishTime, NULL);
    printf("Start time: %lu.%lu\nFinish time: %lu.%lu\n", startTime.tv_sec, startTime.tv_usec, finishTime.tv_sec, finishTime.tv_usec);
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
