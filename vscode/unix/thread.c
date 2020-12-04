#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

pthread_t ntid;
struct m_struct{
    int id;
    char name[16];
};
struct m_struct y;
void pexit_a(void * arg);
void pexit_b(void * arg);
void* printid(void *arg);

int main(int argc,char ** argv)
{
    int num=5;
    int err = pthread_create(&ntid,NULL,printid,(void *)&num);
    // pthread_cancel(ntid);
    //printf("%d",NULL);
    //printid(NULL);
    struct m_struct* result;
    err=pthread_join(ntid,(void *)&result);
    if(err){
        printf("thread_join called failed, error num : %d\n",err);
        exit(3);
    }
    printf("child exit : y.id %d , y.name %s\n",result->id,result->name);
    puts("done");
    sleep(1);
    return 0;
}
void pexit_a(void * arg){
    printf("exit a %s\n",(char *)arg);
}
void pexit_b(void * arg){
    printf("exit b %s\n",(char *)arg);
}
void* printid(void *arg){
    
    pthread_cleanup_push(pexit_a,"lala");//i don't know why these two line lead to complie error
    pthread_cleanup_push(pexit_b,"lblb");//online they say i omit crakets
    pid_t pid;
    pthread_t tid;
    pid=getpid();
    tid=pthread_self();
    if(arg!=NULL)
    {
        printf("pid %lu tid %lu (0x%lx)\nhave a num %d\n",(unsigned long)pid,(unsigned long)tid,(unsigned long)tid,*(int*)arg);
        y.id=25;
        strcpy(y.name,"yikarus");
        pthread_exit((void *)&y);
    }
    else
    {
        printf("pid %lu tid %lu (0x%lx)\n",(unsigned long)pid,(unsigned long)tid,(unsigned long)tid);
        pthread_exit(0);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);//really i omit brackets,pthread_cleanup_push() and pthread_cleanup_pop are Macros
}