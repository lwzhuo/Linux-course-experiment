#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define BUFF_SIZE 128
#define NUM 512
int write1(int fd)
{
    int sum=0,n;
    char buff[BUFF_SIZE]="12345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678";//128B  
    for(int i=0;i<NUM-1;i++)//128*128B=16384
        sum += (write(fd,buff,strlen(buff)));
    return sum;
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