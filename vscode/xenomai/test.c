 #include <native/task.h>
#include <native/timer.h> 
#define TASK_PRIO 99 /* Highest RT priority */ 
#define TASK_MODE 0 /* No flags */ 
#define TASK_STKSZ 0 /* Stack size (use default one) */ 
#define TASK_PERIOD 100000 /* 100 usc period */ 
RT_TASK task_desc; 
void sampling_task (void *cookie) 
{ 
        unsigned long overruns; 
        int err; 
        /* The task will undergo a 100 uscperiodic timeline. */ 
        err = rt_task_set_periodic(NULL,TM_NOW,TASK_PERIOD); 
        for (;;) { 
        err =rt_task_wait_period(&overruns); 
        if (err) 
            break; 
        /* Work for the current period*/ 
        } 
} 
int main (int argc, char *argv[]) 
{ 
        int err; 
        /* Disable paging for this program'smemory. */ 
        // ... 
        /* Create a real-time task */ 
        err =rt_task_create(&task_desc, 
        "MyTaskName", 
        TASK_STKSZ, 
        TASK_PRIO, 
        TASK_MODE); 
        if (!err) 
        /* If successfully created, start thetask. */ 
    rt_task_start(&task_desc,&sampling_task,NULL); 
        
}
