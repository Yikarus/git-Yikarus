#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char ** argv){
    struct sockaddr_in _sock;
    inet_pton(AF_INET,"127.0.0.1",&_sock);
    printf("%d:",in6addr_any);
    char *p=getenv("PATH");
    printf("%s",p);
    
}