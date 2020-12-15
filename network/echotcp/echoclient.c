#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/signal.h>
#include<string.h>
#include<stdlib.h>
char sendline[1024],recvline[1024];
void sig_handle2(int signum){
    printf("sendline:%s\n",sendline);
    printf("recvline:%s\n",recvline);
}
void str_echo(FILE* fp,int sockfd){
    while(1){
        bzero(sendline,1024);
        bzero(recvline,1024);
        printf("send: ");
        if(fgets(sendline,1024,fp)==NULL){
            puts("recv null exit"); 
            exit(1); 
        }
        int n=send(sockfd,sendline,strlen(sendline),0);
        if(-1==n)
        {
            puts("send msgs failure");
            return;
        }
        printf("received: ");
        if(read(sockfd,recvline,1024)==0){
            puts("client:server terminated permaturely");
            exit(1);
        }
        fputs(recvline,stdout);
    }
}
void sig_handle(int signum){
    puts("sig sleep");//simulate refuse attack
    sleep(10);
}
int main(int argc ,char ** argv){
    puts("client init");
    int sockfd;
    signal(SIGUSR1,sig_handle);
    signal(SIGUSR2,sig_handle2);
    struct sockaddr_in servaddr;
    if(argc!=3){
        return 1;
    }
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)puts("socket err");
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[2]));
    // servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr.s_addr);
    int err=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(-1==err)puts("connect err");
    else if(0==err)puts("connect successfully");
    str_echo(stdin,sockfd);
    exit(0);
}