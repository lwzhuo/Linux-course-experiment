#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	/*处理参数个数开始*/
	if(argc!=2)
	{
		printf("usage:cmd1 [integer]\n");
		exit(1);
	}
	/*处理参数个数结束*/

	int num = atoi(argv[1]);
	if(!strcmp(argv[1],"0"))//处理参数0的情况
	{
		printf("0! = 1\n");
		return 0;
	}	
	if(num == 0)
	{
		printf("cmd1 : argument not an integer!\n");
		exit(3);
	}
	if(num>15)//处理溢出情况
		num = 15;
    for(int i = 1,sum = 1;i<=num;i++){
        printf("%d! = %d\n",i,sum*=i);
		usleep(200000);//0.2s
	}
    return 0;
}
