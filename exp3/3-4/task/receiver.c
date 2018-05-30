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
        //创建失败处理 使用打开而不是创建
        mutex = sem_open("/mutex",O_RDWR);
        full = sem_open("/full",O_RDWR);
        empty = sem_open("/empty",O_RDWR);
        if(mutex==SEM_FAILED||full==SEM_FAILED||empty==SEM_FAILED)
            printf("sem_open error\n");
    }
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
    while(1){
        char buff[100];
        sem_wait(full);
        sem_wait(mutex);
        if(*data==SENDER_EXIT){
            //sender退出处理
            sem_post(mutex);
            sem_post(empty);
            break;
        }
        if(*data != INIT){//判断共享数据段是否为初始状态
            if(*data == SENDER){
                printf("message from sender %s\n",data+1);
                strcpy(data+1,"over");
                *data = RECEIVER;
            } 
        }
        sem_post(mutex);
        sem_post(empty);
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