#include<iostream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<unistd.h>
#include<vector>
int main(){
    int _sock=socket(AF_INET,SOCK_STREAM,0);
    if(-1==_sock){
        std::cout<<"creat error"<<std::endl;
        return -1;
    }
    
    sockaddr_in _sockaddr;
    _sockaddr.sin_family=AF_INET;
    _sockaddr.sin_port=htons(12345);
    _sockaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(-1==bind(_sock,(sockaddr *)&_sockaddr,sizeof(_sockaddr)))std::cout<<"bind error"<<std::endl;

    if(-1==listen(_sock,6))std::cout<<"listen error"<<std::endl;
    
    while(1){
        sockaddr_in _cockaddr;
        socklen_t len;
        int _cock=accept(_sock,(sockaddr *)&_cockaddr,&len);
        if(-1==_cock){
            std::cout<<"failed to accept"<<std::endl;
            continue;
        }
        std::cout<<"client connected : "<<inet_ntoa(_cockaddr.sin_addr)<<std::endl;
        int p=fork();
        if(p=0)
        {
            execl("/home/yikarus/vscode/fork_funcs","fork_funcs",)
        }
        std::cout<<"new process created to handle socket "<<_cock<<std::endl;
    }
    
    
    // close(_sock);
    return 0;
}