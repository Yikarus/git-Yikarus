#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
void err_quit(char *msgs){
    printf("error: %s !\n",msgs);
    exit(0);
}
int main(int argc,char **argv){
    if(argc!=3)err_quit("<usage>: program netaddress port");
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==clientfd)err_quit("socket create failure");
    struct sockaddr_in clientaddr;
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_port=htons(atoi(argv[2]));
    clientaddr.sin_addr.s_addr=inet_addr(argv[1]);
    int sockfd=connect(clientfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
    if(-1==sockfd)err_quit("connect server failure");
    puts("start ping");
    int i=0;
    char buf[1024];
    
    while(1){
        bzero(buf,1024);
        i++;
        sprintf(buf,"hello times:%d",i);        
        int n=send(clientfd,buf,sizeof(buf),0);
        if(-1==n)err_quit("ping error");
        sleep(1);
    }
}