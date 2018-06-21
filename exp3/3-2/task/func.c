#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define BUFF_SIZE 128
#define NUM 512
int write1(int fd)
{
    char buff[BUFF_SIZE]="this is p1\n";
    return write(fd,buff,strlen(buff));
}
int write2(int fd)
{
    char buff[BUFF_SIZE]="this is p2\n";
    return write(fd,buff,strlen(buff));
}
int write3(int fd)
{
    char buff[BUFF_SIZE]="this is p3\n";
    return write(fd,buff,strlen(buff));
}
