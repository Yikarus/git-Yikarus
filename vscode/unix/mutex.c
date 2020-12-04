#include<stdio.h>
#include<pthread.h>
#include<time.h>
int main(int argc,char ** argv){
    pthread_mutex_t lock;
    pthread_mutex_init(&lock,NULL);
    pthread_mutex_lock(&lock);//this case and different  order of lock will lead to dead lock
    // pthread_mutex_lock(&lock);//we can use pthread_mutex_trylock(),if the resource is locked then release its own resource and try again later.
    struct timespec tout;
    struct tm *tmp;
    // pthread_mutex_timedlock() function can wait to the time you input if the lock is hold.
    char buf[64];
    clock_gettime(CLOCK_REALTIME,&tout);
    tmp=localtime(&tout.tv_sec);
    strftime(buf,sizeof(buf),"%r",tmp);
    printf("current time is %s\n",buf);
    // pthread_mutex_unlock(&lock);

    tout.tv_sec+=2;
    pthread_mutex_timedlock(&lock,&tout);
    clock_gettime(CLOCK_REALTIME,&tout);
    tmp=localtime(&tout.tv_sec);
    strftime(buf,sizeof(buf),"%r",tmp);
    printf("current time is %s\n",buf);
    pthread_mutex_unlock(&lock);
    return 0;  
}