#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>
#include<arpa/inet.h>
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
        // sleep(1);
        // puts("new loop");
        ready=poll(pollfdd,max+1,TIMEOUT);
        // printf("poll ready num : %d\n",ready);
        if(pollfdd[0].revents&POLLRDNORM){
            // puts("ready to accept");
            socklen_t len;
            clifd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
            if(clifd==-1)err_quit("accept error");
            
            printf("connect remote client: %s \n",inet_ntoa(cliaddr.sin_addr));
            for(num=1;num<OPEN_MAX;num++)
                if(pollfdd[num].fd<0)
                {
                    pollfdd[num].fd=clifd;
                    break;
                }
                    pollfdd[num].events=POLLRDNORM;
            
            if(num==OPEN_MAX)err_quit("out of range of connection");
            if(num>max)max=num;
            if(--ready<=0)
                continue;
            // puts("quit accept");

        }
        //if(ready<=0)continue;
        // printf("max num: %d \n",max);
        for(int i=1;i<=max;i++){
            if(pollfdd[i].fd<0)
                continue;
            // printf("search %d pollfd ,fd:%d\n",i,pollfdd[i].fd);
            if(pollfdd[i].revents&POLLRDNORM)//see how to set revents because pollfd is ok not released,but followed message losen.
            {
                int recvlen;
                bzero(buf,1024);
                if(-1==(recvlen=recv(pollfdd[i].fd,buf,1024,0)))err_quit("recv err");
                else if(0==recvlen)
                {
                    printf("client exit\n");
                    close(pollfdd[i].fd);
                    pollfdd[i].fd=-1;
                    
                }
                else
                {
                    send(pollfdd[i].fd,buf,sizeof(buf),0);
                }
                
                // printf("in loop %d : pollfd %d\n",i,pollfdd[i].fd);

                printf("received : %s \n",buf);
                
            }
            if(--ready<=0)
                break;
            
        }
        // for(int n=0;n<=max;n++)
        //     printf("pollfd[%d].fd:%d\n",n,pollfdd[n].fd);
        // puts("end of one loop");
        //now can not serve to mutithread
    }
}