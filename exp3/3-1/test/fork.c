#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
int main()
{
    pid_t pid;
    char *args[10]={NULL};
    args[0]="cmd1";
    args[1]="10";
    printf("%s\n%s\n%s\n",args[0],args[1],args[2]);
    if((pid=fork())<0)
        printf("error\n");
    else if(pid==0)
        if(execv("cmd1",args)<0)
        printf("errno %s\n",strerror(errno));
    return 0;
}