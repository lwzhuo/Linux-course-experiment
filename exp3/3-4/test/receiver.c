#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<fcntl.h>
int main()
{
    char *str;
    key_t key = ftok("./sender.c",10);
    int s;
    if(key<0)
        printf("ftok error\n");
    s = shmget(key,100,IPC_CREAT|S_IRUSR|S_IWUSR);
    if(s<0)
        printf("shmget error\n");
    str = (char *)shmat(s,0,0);
    printf("%s\n",str);
    return 0;
}