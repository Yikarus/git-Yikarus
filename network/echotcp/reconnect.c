#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc, char ** argv){
    struct sockaddr_in seraddr;
    seraddr.sin_family=AF_INET;
    
}