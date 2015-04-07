#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    close(sockfd);
    return 0;
}
