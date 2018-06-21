#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<string.h>
#define MSG_SIZE 512
//全局变量
int msqid;
size_t msgsize;
//消息格式 mtype表明数据类型 mtext用于存储数据
struct msg{
    long mtype;
    char mtext[MSG_SIZE];
};
struct msg mymsg;
sem_t mutex,empty,full;//三个信号量

//sender程序
void *sender(void *arg){
    char buff[MSG_SIZE];
    int flag = 0;//标志位，用于判断是否结束。初始为0;1为退出
    while(1){
//wait操作
        sem_wait(&empty);
        sem_wait(&mutex);
        if((msgrcv(msqid,&mymsg,MSG_SIZE,1,IPC_NOWAIT))>0)//从队列中读取消息，这里是读取receiver反馈的消息
            puts(mymsg.mtext);
        if(flag){//判断标志位，为1则结束此线程
            printf("\033[31m\033[1mSender exit!\033[0m\n");
            return;
        }
        printf("\033[33m\033[1mSender input:\033[0m");//用户输入消息
        gets(buff);
        if(!strcmp(buff,"exit"))//判断内容是否为"exit"
            flag=1;             //如果为exit设置标志位
//组装消息结构        
        mymsg.mtype=1;//组装数据类型
        strcpy(mymsg.mtext,buff);//组装消息内容
        if(msgsnd(msqid,&mymsg,MSG_SIZE,0)<0)//发送消息
            printf("\033[31m\033[1mSender:msgsend error!\033[0m\n");
//signal操作
        sem_post(&mutex);
        sem_post(&full);
    }
}
//receiver程序
void *receiver(void *arg){
    char buff[MSG_SIZE];
    int flag = 0;//标志位，用于判断是否结束。初始为0;1为退出
    while(1){
//wait操作
        sem_wait(&full);
        sem_wait(&mutex);
        if((msgrcv(msqid,&mymsg,MSG_SIZE,1,0))<0)//从队列中读取消息，这里是读取sender发送的消息
            printf("\033[31m\033[1mReceiver:msgsend error!\033[0m\n");
        else{
            if(strcmp(mymsg.mtext,"exit"))//判断内容是否为"exit"
                snprintf(buff,512,"\033[36m\033[01mReceiver reply \"%s\"\033[0m:over",mymsg.mtext);
            else{//如果为exit设置标志位
                flag = 1;
                snprintf(buff,512,"\033[36m\033[01mReceiver reply \"%s\"\033[0m:over\n\033[31m\033[01mReceiver exit!\033[0m",mymsg.mtext);
            }
//组装消息结构
            strcpy(mymsg.mtext,buff);
            mymsg.mtype=1;
            msgsnd(msqid,&mymsg,MSG_SIZE,0);//发送消息
        }
//signal操作
        sem_post(&mutex);
        sem_post(&empty);
        if(flag)//判断标志位，为1则结束此线程
            return;
    }
}
int main()
{
    pthread_t Sender,Receiver;
//创建无名信号量
    sem_init(&mutex,0,1);
    sem_init(&full,0,0);
    sem_init(&empty,0,1);
//创建消息队列
    //使用IPC_PRIVATE和IPC_CREAT参数表示创建新的消息队列
    //这里很坑，除了表示创建的flag，读写权限0660也要加上
    if((msqid=msgget(IPC_PRIVATE,IPC_CREAT|0660))<0)
        printf("megget error\n");
//创建线程
    pthread_create(&Sender,NULL,sender,NULL);//创建sender线程，执行sender函数,不传入参数，不对线程属性进行定制
    pthread_create(&Receiver,NULL,receiver,NULL);//创建receiver线程，执行receiver函数,不传入参数，不对线程属性进行定制
    sleep(1);
//阻塞调用线程，保证sender和receiver线程先于进程结束。
    pthread_join(Sender,NULL);
    pthread_join(Receiver,NULL);
//删除消息队列 IPC_RMID
    msgctl(msqid,IPC_RMID,NULL);
//销毁三个无名信号量
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
