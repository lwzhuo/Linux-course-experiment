#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<string.h>
#define MSG_SIZE 512
int main()
{
    key_t key;
    int msqid;
    size_t msgsize;
    struct msg{
        long mtype;
        char mtext[MSG_SIZE];
    };
    struct msg mymsg,recmsg;
    mymsg.mtype = 1;
    strcpy(mymsg.mtext,"hello world");
    key = ftok("/msgqueue.c",12);
    // msqid = msgget(IPC_PRIVATE,IPC_CREAT|IPC_EXCL);
    if((msqid=msgget(IPC_PRIVATE,IPC_CREAT|0660))<0)//很坑这里，除了表示创建的flag，读写权限也要加上
        printf("megget error\n");
    if(msgsnd(msqid,&mymsg,200,0)<0)
        printf("msgsend error\n");
    msgsize = msgrcv(msqid,&recmsg,MSG_SIZE,1,0);
    printf("%d %s\n",msgsize,recmsg.mtext);
    msgsize = msgrcv(msqid,&recmsg,MSG_SIZE,1,0);
    printf("%d %s\n",msgsize,recmsg.mtext);
    msgctl(msqid,IPC_RMID,NULL);
    return 0;
}