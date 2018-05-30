#include<stdio.h>
#include<sys/ipc.h>
int main()
{
    key_t key = ftok("/bin/bash",15);
    printf("%d\n",(int)key);
    return 0;
}