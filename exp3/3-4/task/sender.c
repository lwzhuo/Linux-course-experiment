#include<stdio.h>
#include<semaphore.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<fcntl.h>
#define MSG_MEM_SIZE 1024 //消息数据存储大小1k
#define SEM_MEM_SIZE 64   //信号量存储大小16B
#define SENDER '1'
#define RECEIVER '0'
#define INIT '2'
#define SENDER_EXIT '3'
int main()
{
//3个信号量
    sem_t *mutex;//共享存储临界资源(1)
    sem_t *full;//满(0)
    sem_t *empty;//空(1)
    char *data;
    key_t msg_key;
    int msg_smd;
    msg_key = ftok("./sender.c",10);//生成消息数据的key
    if(msg_key<0){
        printf("ftok error\n");
        return 0;
    }
//信号量创建并初始化
    mutex = sem_open("/mutex",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1);
    full = sem_open("/full",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0);
    empty = sem_open("/empty",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1);
    if(mutex==SEM_FAILED||full==SEM_FAILED||empty==SEM_FAILED){
        printf("sem_open error,retry\n");
        mutex = sem_open("/mutex",O_RDWR);
        full = sem_open("/full",O_RDWR);
        empty = sem_open("/empty",O_RDWR);
    }
//创建共享存储段 消息数据
    msg_smd = shmget(msg_key,MSG_MEM_SIZE,IPC_CREAT|S_IRUSR|S_IWUSR);
    if(msg_smd<0){
        printf("shmget error\n");
        return 0;
    }
    data = (char*)shmat(msg_smd,0,0);//获得存储区首位地址的指针
    if(data<0){
        printf("shmat error\n");
        return 0;
    }
    *data = INIT;//初始化发送标识段
    printf("input 'exit' to exit\n");
    while(1){
        char buff[100];
        sem_wait(empty);
        sem_wait(mutex);
        if(*data != INIT)//判断共享数据段是否为初始状态
            printf("message from receiver %s\n",data+1);
        printf("Sender input:");
        scanf("%s",buff);
        if(!strcmp(buff,"exit")){//处理退出的收尾工作
            printf("exit!\n");
            *data=SENDER_EXIT;
            sem_post(mutex);
            sem_post(full);
            break;
        }
        *data = SENDER;//标识消息发送对象
        strcpy(data+1,buff);
        sem_post(mutex);
        sem_post(full);
    }
//解除信号量引用
    sem_unlink("/mutex");
    sem_unlink("/full");
    sem_unlink("/empty");
//分离并删除共享存储区
    shmdt(data);
    shmctl(msg_smd,IPC_RMID,0);
    return 0;
}