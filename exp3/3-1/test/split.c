// 字符串分割测试
#include<stdio.h>
#include<string.h>
int main()
{
    // char str[] = "hello world sss hhh sss !";
    char str[100];
    char *token;
    gets(str);
    token=strtok(str," ");
    if(token)
    printf("%s\n",token);
    while(token=strtok(NULL," "))
        printf("%s\n",token);
    return 0;
}