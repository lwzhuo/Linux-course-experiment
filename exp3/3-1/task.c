#include<stdio.h>
#include<stdlib.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

void print_head()
{
// whoami@umane -n : pwd $
    FILE *fp;
    char loginname[20];
    char hostname[20];
    char pathname[100];
    fp = popen("echo -n `whoami`","r");
    fgets(loginname,sizeof(loginname),fp);
    fp = popen("echo -n `uname -n`","r");
    fgets(hostname,sizeof(hostname),fp);
    fp = popen("echo -n `pwd`","r");
    fgets(pathname,sizeof(pathname),fp);

    printf("\033[32m\033[01m%s@%s:%s$ \033[0m",loginname,hostname,pathname);
}

char *getpwd()
{
    FILE *fp;
    char pathname[100];
    fp = popen("echo -n `pwd`","r");
    fgets(pathname,sizeof(pathname),fp);
    return pathname;
}

int main()
{
    int flag = 1;//标志位，标识各种状态
    char *arg=NULL;//初始化各种指针
    char *args[10]={NULL};
    char *ins=NULL;
    pid_t pid;
    pid_t lastpid;

    system("clear");
    printf("enter virtual shell,input exit to exit!\n");
    while(flag){
        char buffer[100]={0};
        print_head();//打印虚拟shell信息
        gets(buffer);//获取命令 切记 字符串之间有空格用gets
        if(!strcmp(buffer,"exit"))//检测是否为退出命令
            flag = 0;
        else{
            int i = 1;
            ins = strtok(buffer," ");//取出命令
            if(!ins)//判断是否输入为空
                continue;
            args[0] = ins;//第0号元素单独处理
            while(arg = strtok(NULL," ")){//循环取出参数
                if(i==10){//参数个数检测，限制最多参数个数
                    printf("%s:too many argument\n",ins);
                    flag = 2;
                    break;
                }
                args[i++] = arg;
            }
            if(flag!=2){//没有出现参数个数异常
                if((pid=fork())<0)
                    printf("fork error\n");
                else if(pid==0){
                    if(execv(ins,args)<0){
                        if(errno==2)//处理command not found情况(No such file or directory)
                            printf("%s: Command not found\n",ins);
                        else
                            printf("exec error %d %s\n",errno,strerror(errno));
                        exit(errno);
                    }
                }
                else{
                    lastpid=0;
                    waitpid(pid,NULL,0);
                    // while((lastpid=wait(NULL))!=pid&&lastpid!=-1);//等待子进程结束
                }
            }
        }
    }
    system("clear");
    return 0;
}