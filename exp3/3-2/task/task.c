#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<string.h>
#include"func.h"
#define BUFF_SIZE 65536//64k
#define MEM_SIZE 16
int main()
{
    pid_t pid1,pid2,pid3;
    key_t key;
    int fd[2];
    int smd;
    char buff[BUFF_SIZE];
    sem_t *pi;
    if(pipe(fd)<0)//创建管道
        printf("pipe error\n");

    pi = sem_open("/P",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1);//创建新的信号量 初始值1
    if(pi==SEM_FAILED){
        printf("sem_open error\n");
        return 0;
    }
/*fork三个子进程*/
/*P1*/
    if((pid1=fork())<0)
        printf("fork error\n");
    else if(pid1 == 0){//子进程
        printf("p1 send message\n");
        sem_wait(pi);//获取临界资源
        close(fd[0]);
        printf("p1 send %d Bytes data\n",write1(fd[1]));
        close(fd[1]);
        sem_post(pi);//释放临界资源
        sem_close(pi);//关闭信号量
        return 0;
    }
/*P2*/
    if((pid2=fork())<0)
        printf("fork error\n");
    else if(pid2 == 0){
        printf("p2 send message\n");
        sem_wait(pi);
        close(fd[0]);
        printf("p2 send %d Bytes data\n",write2(fd[1]));
        close(fd[1]);
        sem_post(pi);
        sem_close(pi);   
        return 0;
    }
/*P3*/
    if((pid3=fork())<0)
        printf("fork error\n");
    else if(pid3 == 0){
        printf("p3 send message\n");
        sem_wait(pi);
        close(fd[0]);
        printf("p3 send %d Bytes data\n",write3(fd[1]));
        close(fd[1]);
        sem_post(pi);
        sem_close(pi);
        return 0;
    }

    waitpid(pid1,NULL,0);//等待子进程结束
    waitpid(pid2,NULL,0);
    waitpid(pid3,NULL,0);
    close(fd[1]);
    printf("\nshow the message from p1 p2 p3\n");
    int n = read(fd[0],buff,BUFF_SIZE);//读取数据 并返回读取字节数
    printf("%s",buff);
    printf("\nrecive %d Bytes\n",n);
    sem_unlink("/P");//解除信号量的引用
    // shmctl(smd,IPC_RMID,0);//删除该共享存储段
    return 0;
}