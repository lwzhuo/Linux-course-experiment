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
#define SENDER_EXIT '3'
int main()
{
//3个信号量
    sem_t *mutex;//共享存储临界资源(1)
    sem_t *receiver_response;//接受者响应(0)
    sem_t *sender_write;//发送者写(1)
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
    receiver_response = sem_open("/receiver_response",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0);
    sender_write = sem_open("/sender_write",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1);
    if(mutex==SEM_FAILED||receiver_response==SEM_FAILED||sender_write==SEM_FAILED){
        //创建失败处理 使用打开而不是创建
        mutex = sem_open("/mutex",O_RDWR);
        receiver_response = sem_open("/receiver_response",O_RDWR);
        sender_write = sem_open("/sender_write",O_RDWR);
        if(mutex==SEM_FAILED||receiver_response==SEM_FAILED||sender_write==SEM_FAILED)
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
        sem_wait(receiver_response);
        sem_wait(mutex);
        if(*data==SENDER_EXIT){
            printf("\033[31m\033[01msender exit\033[0m\n");
            strcpy(data+1,"over");
            *data = RECEIVER;
            //sender退出处理
            sem_post(mutex);
            sem_post(sender_write);
            break;
        }
        if(*data == SENDER){
            printf("\033[36m\033[01mmessage from sender\033[0m: %s\n",data+1);
            strcpy(data+1,"over");
            *data = RECEIVER;
        } 
        sem_post(mutex);
        sem_post(sender_write);
    }
//解除信号量引用
    sem_unlink("/mutex");
    sem_unlink("/receiver_response");
    sem_unlink("/sender_write");
//分离并删除共享存储区
    shmdt(data);
    shmctl(msg_smd,IPC_RMID,0);
    return 0;
}