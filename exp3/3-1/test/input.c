#include<stdio.h>
int main()
{
    char buff[10]={1};
    scanf("%s",buff);
    for(int i=0;i<sizeof(buff);i++)
        printf("%d\n",buff[i]);
    return 0;
}