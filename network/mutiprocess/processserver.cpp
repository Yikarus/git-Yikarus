#include<iostream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<vector>
#include<unistd.h>
#include<unordered_map>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/signal.h>
std::unordered_map<pid_t,int> clientsockets;
char recv_buf[1024];
int main(){
    int _sock=socket(AF_INET,SOCK_STREAM,0);
    if(-1==_sock){
        std::cout<<"creat error"<<std::endl;
        return -1;
    }
    // key_t key=ftok(".",)
    // int r=shmget(getpid(),1024,IPC_CREAT);
    sockaddr_in _sockaddr;
    _sockaddr.sin_family=AF_INET;
    _sockaddr.sin_port=htons(12345);
    _sockaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(-1==bind(_sock,(sockaddr *)&_sockaddr,sizeof(_sockaddr)))std::cout<<"bind error"<<std::endl;

    if(-1==listen(_sock,6))std::cout<<"listen error"<<std::endl;
    std::cout<<"start"<<std::endl;
    std::cout<<_sock<<std::endl;
    int max=_sock;
    while(1){

            sockaddr_in _clieaddr={};
            socklen_t len;
            int _cock=accept(_sock,(sockaddr *)&_clieaddr,(socklen_t * )&len);
            
            if(-1==_cock){
                std::cout<<"accept error"<<std::endl;
                continue;
            }
            int ret=fork();
            if(ret==0)//child
            {
                close(_sock);//close useless socket
                sleep(1);
                std::unordered_map<pid_t,int>::iterator index;
                while(1){
                    index=clientsockets.find(getpid());
                    if(index==clientsockets.end())
                    {
                        // std::cout<<"no such client:"<<getpid()<<" exit"<<std::endl;
                        std::cout<<"child client status:"<<std::endl;
                        for(auto i:clientsockets)
                            std::cout<<i.first<<":"<<i.second<<" ";
                        std::cout<<std::endl;
                        sleep(1);
                        continue;
                    }
                    break;
                }
                std::cout<<"child process "<<getpid()<<" start to handle _cock "<<index->second<<std::endl;
                while(1)
                {
                    int num=recv(index->second,recv_buf,1024,0);
                    if(num<0)
                    {
                        std::cout<<"recv error"<<std::endl;
                        exit(2);
                    }
                    std::cout<<"child "<<getpid()<<" receive "<<num<<" bytes:"<<recv_buf<<std::endl;
                }
            }
            clientsockets.insert(std::pair<pid_t,int>(ret,_cock));
            std::cout<<"client connected : "<<inet_ntoa(_clieaddr.sin_addr)<<std::endl;
            for(auto i:clientsockets)
                std::cout<<i.first<<":"<<i.second<<" ";
            std::cout<<std::endl;
            sleep(1);
}
    
    while (1)
    {
        int status;
        int childid=wait(&status);
        if(childid<0)
            break;
        if(status==2){
            std::cout<<"child recv error"<<std::endl;
        }
        if(status==1){
            std::cout<<"child can not find discriptor"<<std::endl;
        }
        clientsockets.erase(childid);
        
    }
    
    std::cout<<"done"<<std::endl;
    // close(_sock);
    return 0;
}