/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
     listen(sockfd,500);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     n = 101;
     int count = 0;
     while (n > 100) {
         memset(buffer, 0, sizeof(buffer));
         n = read(newsockfd, buffer, sizeof(buffer));
         if (n < 0) error("ERROR reading from socket");
         printf("R: %cx%d %d    ",buffer[0], strlen(buffer), count);
         n = write(newsockfd,&buffer,strlen(buffer));
         if (n < 0) error("ERROR writing to socket");
         printf("S: %cx%d\n",buffer[0], strlen(buffer));
         count++;
     }
     close(newsockfd);
     close(sockfd);
     return 0; 
}