#include<stdio.h>
#include<pthread.h>
void * func(void* arg){
    puts("detached");
}
int main(int argc,char ** argv){
    int err;
    pthread_t tid;
    pthread_attr_t attr;
    err=pthread_attr_init(&attr);
    if(err!=0)
    {
        puts("init attr error");
        return 1;
    }
    int isable;
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    err=pthread_attr_getdetachstate(&attr,&isable);
    void * addr;
    size_t len;
    if(!pthread_attr_getstack(&attr,&addr,&len))
    printf("attr stack top address :%p size %lx\n",addr,len);
    if(isable==PTHREAD_CREATE_DETACHED)puts("pthread_attr_detached");
    else if(isable==PTHREAD_CREATE_JOINABLE)puts("pthread_attr_joinable");
    if(err==0)
        err=pthread_create(&tid,&attr,func,0);
    pthread_attr_destroy(&attr);
    return err;
}