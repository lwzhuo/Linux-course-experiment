#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>
#include<linux/sched.h>
#include<linux/types.h>
#include<linux/list.h>
#include<linux/sched/signal.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
static int pid=-1;
module_param(pid,uint,0644);

static const char * const task_state_array[] = {
	"R",		/* 0x00  (running)*/
	"S",		/* 0x01  (sleeping)*/
	"D",		/* 0x02  (disk sleep)*/
	"T",		/* 0x04  (stopped)*/
	"t",		/* 0x08  (tracing stop)*/
	"X",		/* 0x10  (dead)*/
	"Z",		/* 0x20  (zombie)*/
	"P",		/* 0x40  (parked)*/
	"I",		/* 0x80  (idle)*/
};

void static show(struct task_struct *task)
{
    printk("%ld %s %s\n",(long)task->pid,task->comm,task_state_array[task_state_index(task)]);
}

static int task_init(void)
{   
	if(pid != -1)
	{
		struct task_struct *parent;
		struct task_struct *this;
		struct task_struct *s;
		// struct list_head children_list;
		struct list_head *lh;
		struct pid *p;

		p = find_get_pid(pid);
		if(p==NULL)
		{
			printk(KERN_ALERT"inncorrect pid!\n");
			return -1;
		}
		this = pid_task(p,PIDTYPE_PID);

		parent = this->real_parent;
		// children_list = this->children;
		// sibling_list = this->sibling;

		show(this);
		printk("parent\n");
		show(parent);
		
		printk("children\n");
		list_for_each(lh,&this->children){
		    s = list_entry(lh,struct task_struct,sibling);
		    show(s);
		}
		printk("sibling\n");
		// list_for_each(lh,&parent->children){
		//     s = list_entry(lh,struct task_struct,sibling);
		// 	if(s->pid!=pid)
		//     	show(s);
		// }
		list_for_each_entry(s,&parent->children,sibling){
			show(s);
		}
	}
	else
		printk(KERN_ALERT"please input pid");
    return 0;
}

static void task_exit(void)
{
    printk(KERN_ALERT"exit!");
}

module_init(task_init);
module_exit(task_exit);
