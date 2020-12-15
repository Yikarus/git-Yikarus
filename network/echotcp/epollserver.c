#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<stdlib.h>
void err_quit(char *msgs){
    printf("error: %s !\n",msgs);
    exit(1);
}
int main(int argc,char **argv){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==listenfd)err_quit("socket created failure");
    struct sockaddr_in servaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr=INADDR_ANY;
    if(-1==bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)))err_quit("bind failure");
    if(-1==listen(listenfd,5))err_quit("listen failure");
    
    int client=accept()
}
