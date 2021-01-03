#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

void err_quit(char *msgs){
    printf("error: %s !\n",msgs);
    exit(1);
}
void err_continue(char *msgs){
    printf("error: %s !\n",msgs);
}
int main(int argc,char **argv){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    int one=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&one,sizeof(one));
    if(-1==listenfd)err_quit("socket created failure");
    int clientfd;
    char buf[1024];
    socklen_t len;
    struct sockaddr_in servaddr,clientaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr=INADDR_ANY;
    if(-1==bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)))err_quit("bind failure");
    if(-1==listen(listenfd,5))err_quit("listen failure");
    struct epoll_event ev,events[10];
    int epollfd=epoll_create1(0);
    if(-1==epollfd)err_quit("epollfd created failure");
    ev.events=EPOLLIN;
    ev.data.fd=listenfd;
    if(-1==epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev))err_quit("epoll ctl failure");
    while(1){
        int nfds=epoll_wait(epollfd,events,10,-1);
        if(-1==nfds)err_quit("epoll wait failure");
        for(int i=0;i<nfds;i++){
            bzero(buf,1024);
            if(events[i].data.fd==listenfd){
                clientfd=accept(listenfd,(struct sockaddr*)&clientaddr,&len);
                ev.events=EPOLLIN|EPOLLET;
                ev.data.fd=clientfd;
                if(-1==epoll_ctl(epollfd,EPOLL_CTL_ADD,clientfd,&ev))err_quit("client add failure");
                printf("new client connect : %s \n",inet_ntoa(clientaddr.sin_addr));
            }
            else
            {
                ssize_t num;
                if((num=read(events[i].data.fd,buf,1024))==-1)err_quit("read failure in client");
                if(-1==getpeername(events[i].data.fd,(struct sockaddr *)&clientaddr,&len))err_continue("get peername failure");
                if(num==0){
                    printf("a client exit: %s \n",inet_ntoa(clientaddr.sin_addr));
                    if(-1==epoll_ctl(epollfd,EPOLL_CTL_DEL,events[i].data.fd,&ev))err_quit("epoll ctl failure");
                }
                else
                {
                    printf("receive %d bytes from %s : %s",num,inet_ntoa(clientaddr.sin_addr),buf);
                    if(-1==send(events[i].data.fd,buf,sizeof(buf),0))err_continue("send to client failure");
                }
                
            }
            
        }
    }
}
