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

void process(int sockfd){//get num=xxx- from sockfd and return xxx*3-1
    char buffer[1027];
    char *begin,*end;
    int val;
    bzero(buffer,sizeof(buffer));
    read(sockfd,buffer,sizeof(buffer)-1);
    begin = strstr(buffer,"num=");
    if(begin!=NULL){
        end = strstr(begin+4,"-");
    }
    if(begin==NULL || end==NULL){
        write(sockfd,"Usage: num=xxx-\n",16);
        return;
    }
    sscanf(begin,"num=%d-",&val);
    sprintf(buffer,"result=%d\n",val*3-1);
    usleep(320000);// simulate IO delay
    write(sockfd,buffer,strlen(buffer));    
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno,iSetOption=1;
    int count;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    count = 0;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,sizeof(iSetOption));//stop waiting after close(sockfd);

    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr)); //string.h
    portno = atoi(argv[1]);// stdlib.h  string to integer
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,8);
    clilen = sizeof(cli_addr);
    while(1){
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
        if (newsockfd < 0) 
          error("ERROR on accept");

        process(newsockfd);
        /*
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");     
        */
        close(newsockfd);
        printf("Work%4d done!",count++);
    }
    close(sockfd);
    return 0; 
}
