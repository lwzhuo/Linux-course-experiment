#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
int a=0;
struct args{
    int id;
};
int threadlist[10]={0};
void* add(void * arg)
{
    int i=0;
    struct args *ar=(struct args *)arg;
    for(i=0;i<10;i++)
        printf("thread %d: a=%d\n",ar->id,a++);
    threadlist[ar->id-1]=1;
}
int main()
{
    pthread_t p;
    for(int i=0;i<10;i++){
        struct args a;
        a.id = i+1;
        pthread_create(&p,NULL,add,(void *)&a); 
    }
    // if(pthread_join(p,NULL))
    //     printf("thread join fail\n");
    for(int i=0;i<10;i++)
        if(threadlist[i]==0){
            i=0;
            continue;
        }
    printf("*****a=%d*****\n",a);

    return 0;
}