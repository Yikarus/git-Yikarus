#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char ** argv){
    struct sockaddr_in seraddr;
    if(argc<3)return -1;
    
    while(1){
        seraddr.sin_family=AF_INET;
        seraddr.sin_port=htons(atoi(argv[2]));
        seraddr.sin_addr.s_addr=inet_addr(argv[1]);
        int sfd=socket(AF_INET,SOCK_STREAM,0);
        int one=1;
        setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&one,sizeof(one));
        if(-1==sfd){
           printf("socket created err !\n");
            return -2;
        }
        int err;
        err=connect(sfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
        if(-1==err){
            printf("connect err\n");
            sleep(1);
            continue;
        }
        printf("connect successfully\n");
        char sendbuf[1024];
        char recvbuf[1024];
        // int nn;
        while(1){
            fgets(sendbuf,1024,stdin);
            // printf("%d\n",nn);
            // strcpy(sendbuf+nn,'\0');
            err=send(sfd,sendbuf,1024,0);
            if(-1==err){
                printf("send error \n");
                return -4;
            }
            printf("sended : %s \n",sendbuf);
            bzero(sendbuf,1024);
            err=recv(sfd,recvbuf,1024,0);
            if(-1==err){
                printf("recv error \n");
                return -5;
            }
            if(0==err){
                printf("disconnectted\n");
                break;
            }
        }
        printf("reconnecting\n");
        close(sfd);
    }
    
    return 0;
}