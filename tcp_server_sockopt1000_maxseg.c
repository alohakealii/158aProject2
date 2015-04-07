#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdbool.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[64000];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     
     if (sockfd < 0) 
        error("ERROR opening socket");

    int intarg = 460;
    socklen_t intarg_len = sizeof(intarg);
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, (char *)&intarg, sizeof(intarg)) < 0)
        printf("Error setsockopt\n");

    if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &intarg, &intarg_len) < 0)
        printf("Error getsockopt\n");
    printf("After set and get: intarg = %d  len = %d\n", intarg, sizeof(intarg_len));
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     
     bzero(buffer,256);
     
     //accept loop
     int i;
     for (i = 0; i < 1000; i++) {
        memset(buffer, 0, sizeof(buffer));
        n = read(newsockfd,buffer,sizeof(buffer));
        if (n < 0)
            error("ERROR reading from socket");
        printf("Message: %s %d\n",buffer, i);
        n = write(newsockfd,&buffer,strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
     }
     
     close(newsockfd);
     close(sockfd);
     return 0; 
}
