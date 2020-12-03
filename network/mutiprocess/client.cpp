#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
int main()
{
    int _cock=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in _cockaddr;
    _cockaddr.sin_family=AF_INET;
    _cockaddr.sin_port=htons(12345);
    _cockaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int rescon=connect(_cock,(sockaddr *)&_cockaddr,sizeof(sockaddr));
    if(-1==rescon){
        std::cout<<"connect error"<<std::endl;
        return -1;
    }
    std::cout<<"connect success"<<std::endl;
    
    char send_buf[16]="hello ";
    int count=0;
    while(1){
        send(_cock,send_buf,sizeof(send_buf),0);
        std::cout<<"send "<<++count<<" times"<<std::endl;
        sleep(1);
    }
    
    return 0;
}