#include<iostream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
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
    
    
    
    
    std::vector<int> clients;
    char recv_buf[1024];
    int count =0;
    std::cout<<"start"<<std::endl;
    std::cout<<_sock<<std::endl;
    int max=_sock;
    while(1){
        timeval tm;
        fd_set fd_read;
        tm.tv_sec=1;
        tm.tv_usec=0;
        FD_ZERO(&fd_read);
        FD_SET(_sock,&fd_read);
        for(int i=0;i<clients.size();i++)
            FD_SET(clients[i],&fd_read);
        int isset=select(max+1,&fd_read,0,0,&tm);
        if(isset<0)break;
        
        if(FD_ISSET(_sock,&fd_read))
        {
            FD_CLR(_sock,&fd_read);
            unsigned int len=sizeof(sockaddr);
            sockaddr_in _clieaddr={};
            int _cock=accept(_sock,(sockaddr *)&_clieaddr,(socklen_t * )&len);
            
            if(-1==_cock){
                std::cout<<"accept error"<<std::endl;
                return -1;
            }
            clients.push_back(_cock);
            std::cout<<"client connected : "<<inet_ntoa(_clieaddr.sin_addr)<<std::endl;
        }
        // else{
        //     std::cout<<"no input in "<<_sock<<std::endl;
        // }
        // std::cout<<clients.size()<<std::endl;
        
        for(int i=0;i<clients.size();i++){
 
                if(clients[i]>max)
                    max=clients[i];
                FD_CLR(clients[i],&fd_read);
                int recv_len=recv(clients[i],recv_buf,1024,0);
                if(!recv_len)continue;
                std::cout<<++count<<" times receive "<<recv_len<<"bytes from client:"<<recv_buf<<std::endl;
        }
        
        std::cout<<"proceeding"<<std::endl;
    }
    std::cout<<"done"<<std::endl;
    // close(_sock);
    return 0;
}