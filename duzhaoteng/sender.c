//sender.c
#include "common.h"
#include <stdio.h>

//key
key_t key;

//共享内存
int shmid;
char * shmptr;
char input[SHM_SIZE];

//semaphore 
sem_t * full;
sem_t * mutex;
                            //semaphore


void Init()
{
    key = KEY_NUM;                  //init key
    shmid  = GetShmId(key);         // init shared memory
    shmptr = shmat(shmid,NULL,0);       // 把共享内存区对象映射到调用进程的地址空间（共享内存标识符，自己决定，read_and_write）
    //semaphore init
    full = sem_open(FULL_NAME,O_CREAT);
    mutex = sem_open(MUTEX_NAME,O_CREAT);
}

void SaveMessage()
{

    P(mutex);                       
    strcpy(shmptr,input); //把输入字符串复制到共享内存中去
    V(mutex);

    V(full);
}

int main(int argc, char const *argv[])
{


    Init();

    /*waiting for user to input message*/

    fgets(input, 1024, stdin);              //输入你要输入的内容 

    SaveMessage();

    printf("Sender:  Process End\n");
    return 0;
}

