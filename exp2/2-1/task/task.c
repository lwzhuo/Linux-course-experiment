#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched/signal.h>
#include<linux/sched.h>
//参考get_task_state代码
static const char * const task_state_array[] = {
	"R (running)",		/* 0x00 */
	"S (sleeping)",		/* 0x01 */
	"D (disk sleep)",	/* 0x02 */
	"T (stopped)",		/* 0x04 */
	"t (tracing stop)",	/* 0x08 */
	"X (dead)",		/* 0x10 */
	"Z (zombie)",		/* 0x20 */
	"P (parked)",		/* 0x40 */
	"I (idle)",		/* 0x80 */
};
static int task_init(void)
{
    printk(KERN_ALERT"module init\n");
    struct task_struct *task;
    printk("%-7s%-20s%-20s%-5s%-5s\n","pid","name","state","prio","parrent_pid");
    for_each_process(task){
        if(task->mm!=NULL)
            printk("%-7d%-20s%-20s%-5d%-5d\n",task->pid,task->comm,task_state_array[task_state_index(task)],task->prio,task->real_parent->pid);
    }
    return 0;
}
static void task_exit(void)
{
    printk(KERN_ALERT"module exit\n");
}

module_init(task_init);
module_exit(task_exit);
MODULE_LICENSE("GPL");
