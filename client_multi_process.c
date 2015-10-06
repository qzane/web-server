#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define PROCESS_NUM 20

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[256];
    int val,count;    
    
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }   
    
    portno = atoi(argv[2]);
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);      
    
    for(count=0;count<PROCESS_NUM && fork()!=0;++count)
        ;//make PROCESS_NUM copies
    
    if(count==PROCESS_NUM){//root process
        while(wait(NULL)!=-1)
            ;
        printf("\nAll finished!\n");
        exit(0);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //printf("Please enter the message: ");
    bzero(buffer,256);
    //fgets(buffer,255,stdin);
    sprintf(buffer,"num=%d-",count);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    //printf("%s\n",buffer);
    sscanf(buffer,"result=%d",&val);
    close(sockfd);
    if(val==count*3-1)
        printf("Process%3d finished!",count);
    else
        printf("\nERROR%3d !!!\n",count);
    fflush(stdout);
    return 0;
}
