#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>
#include<sys/poll.h>
#include<limits.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define LISTEN_NUM 5
#define OPEN_MAX 1024
#define TIMEOUT 100
void err_quit(char * msgs){
    printf("error:");
    puts(msgs);
    exit(-1);
}
int main(int argc,char **argv){
    if(argc!=2)err_quit("<usage> : program_name port");
    int listenfd,clifd,num,ready;
    if(-1==(listenfd=socket(AF_INET,SOCK_STREAM,0)))err_quit("socket create error");
    struct sockaddr_in seraddr,cliaddr;
    seraddr.sin_addr.s_addr=INADDR_ANY;
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(atoi(argv[1]));
    if(-1==bind(listenfd,(struct sockaddr*)&seraddr,sizeof(seraddr)))err_quit("bind error ");
    puts("bind successfully");
    if(-1==listen(listenfd,LISTEN_NUM))err_quit("listen error");
    puts("listen successfully");
    struct pollfd pollfdd[1024];
    char buf[1024];
    pollfdd[0].fd=listenfd;
    pollfdd[0].events=POLLRDNORM;
    for(int i=1;i<OPEN_MAX;i++)
        pollfdd[i].fd=-1;
    int max=0;
    while(1){
        ready=poll(pollfdd,max+1,TIMEOUT);
        if(pollfdd[0].revents&POLLRDNORM){
            socklen_t len;
            clifd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
            for(num=1;num<OPEN_MAX;num++)
                if(pollfdd[num].fd<0)
                {
                    pollfdd[num].fd=clifd;
                    pollfdd[num].events=POLLRDNORM;
                    break;
                }
            if(num==OPEN_MAX)err_quit("out of range of connection");
            if(num>max)max=num;
            if(--ready<=0)
                continue;
        }
        if(ready<=0)continue;
        for(int i=1;i<max;i++){
            if(pollfdd[i].fd<0)
                continue;
            if(pollfdd[i].revents&POLLRDNORM)
            {
                int recvlen;
                bzero(buf,1024);
                if(-1==(recvlen=recv(pollfdd[i].fd,buf,1024,0)))err_quit("recv err");
                if(0==recvlen)
                {
                    printf("client exit\n");
                    close(pollfdd[i].fd);
                    pollfdd[i].fd=-1;
                    
                }
                printf("in loop %d : pollfd %d\n",i,pollfdd[i].fd);

                puts(buf);
                
            }
            if(--ready<=0)
                break;
        }
    }
}