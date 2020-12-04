#include<stdio.h>
#include<signal.h>
#include<unistd.h>
void sig_handle(int num,siginfo_t sigstru,void * rand){
    printf("receive a num %d\n",sigstru.si_int);
    printf("siginfo_t.si_value.sival_int=%d\n",sigstru.si_signo);
}

int main(int argc,char ** argv){
    struct sigaction stdact;
    stdact.sa_flags=SA_SIGINFO;
    stdact.sa_sigaction=sig_handle;
    union sigval value;
    value.sival_int=2;
    value.sival_ptr=2;
    sigaction(SIGINT,&stdact,NULL);
    
    // while(1){
        sleep(1);
        // sigqueue(getpid(),SIGINT,value);
        printf("proceding\n");
        pause();
    // }
    
}