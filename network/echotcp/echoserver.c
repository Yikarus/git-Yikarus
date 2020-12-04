#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/signal.h>
#include<time.h>
#include<errno.h>
#include<stdlib.h>
extern int errno;
void str_echo(int sockfd){
    ssize_t n;
    char buf[1024];
    while(1){
        n=read(sockfd,buf,1024);
        if(n>0)write(sockfd,buf,n);
        else if(n<0&&errno==EINTR)continue;
        else if(n==0)
        {
            puts("clients over");
            close(sockfd);
            exit(1);
        }else if(n<0){
            puts("read error");
            return;
        }
        
        
    }
}
void child_clean(int signo){
    int status;
    int pid=wait(&status);
    printf("child process %d exit\n",pid);
    return;
}
int main(int argc ,char ** argv){
    puts("server init");
    if(signal(SIGCHLD,child_clean)==EINVAL)puts("rigister err");
    int listenfd,connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;   
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd==-1)puts("socket error"); 
    // bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(12345);
    servaddr.sin_addr.s_addr=INADDR_ANY;
    struct tm *mclockname;
    struct timespec mtimename;

    int err=bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(err==-1)puts("bind error");
    err=listen(listenfd,5);
    if(err==-1)puts("bind error");

    for(;;){
        clilen=sizeof(cliaddr);
        // puts("start accept");
        // sleep(3);
        connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
        char name[24];
        inet_ntop(AF_INET,&cliaddr.sin_addr,name,sizeof(cliaddr));
        clock_gettime(CLOCK_REALTIME,&mtimename);
        mclockname=localtime(&mtimename.tv_sec);
        char buf[64];
        strftime(buf,sizeof(buf),"%r",mclockname);
        printf("new client connect :%s %s\n",name,buf);
        if((childpid=fork())==0){
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
    
}