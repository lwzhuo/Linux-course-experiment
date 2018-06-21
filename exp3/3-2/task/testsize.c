#include<stdio.h>
#include<unistd.h>
int main()
{
    int fd[2],num,sum=0;
    if(pipe(fd)<0){
        printf("pipe error\n");
        return -1;
    }
    while(1){
        num = write(fd[1],"a",1);
        if(num<0)
            break;
        sum+=num;
        printf("write %d byte to pipe,%d bytes in total\n",num,sum);
    }
    return 0;
}