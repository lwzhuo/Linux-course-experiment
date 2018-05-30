#include<stdio.h>
#include<unistd.h>

int main()
{
    pid_t pid;
    printf("parent\n");
    printf("f1*****\n");
    if((pid = fork())<0)
        printf("fork error\n");
    else if(pid == 0){
        for(int i=0;i<3;i++)
            printf("f1:%d\n",i);
            return 0;
    }
    else{
        printf("f2*****\n");
        if((pid = fork())<0)
            printf("fork error\n");
        else if(pid == 0){
            for(int i=0;i<3;i++)
                printf("f2:%d\n",i);
                return 0;
        }
        else{
            printf("f3*****\n");
            if((pid = fork())<0)
                printf("fork error\n");
            else if(pid == 0){
                for(int i=0;i<3;i++)
                    printf("f3:%d\n",i);
                return 0;
            }
            else
                printf("*****f3\n");
        }
        printf("*****f1\n");
    }
    
    return 0;
}