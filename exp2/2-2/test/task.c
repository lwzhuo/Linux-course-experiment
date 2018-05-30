#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>

MODULE_LICENSE("GPL");
static char *who;
static int times;
module_param(who,charp,0644);
module_param(times,uint,0644);

static int hello_init(void)
{
    int i;
    for(i=0;i<times;i++)
        printk("%d  %s\n",i,who);
    return 0;
}

static void hello_exit(void)
{
    printk("goodbye,%s\n",who);
}
module_init(hello_init);
module_exit(hello_exit);
