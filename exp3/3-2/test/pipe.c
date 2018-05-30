#include<stdio.h>
#include<unistd.h>
int main()
{
    pid_t pid;
    int fd[2];
    char buff[100];
    if(pipe(fd)<0)
        printf("pipe error\n");
    if((pid=fork())<0)
        printf("fork error\n");
    else if(pid == 0){
        close(fd[0]);
        write(fd[1],"hello",6);
        write(fd[1],"hello",6);
        write(fd[1],"hello\n",6);
        write(fd[1],"hello\n",6);
        write(fd[1],"hello\n",6);
        write(fd[1],"hello\n",6);
        write(fd[1],"hello\n",6);
        return 0;
    }else{
        close(fd[1]);
        read(fd[0],buff,100);
        printf("hhh%s\n",buff);
        for(int i=0;i<33;i++)
            printf("%d\n",buff[i]);
    }
    return 0;
}