//init.c
#include "common.h"

int main(int argc, char const *argv[])
{
    key_t key;
    int semid;          //semaphore id
    int shmid;          //共享内存 id


    /* 创建 key*/
    key = KEY_NUM;

    /* 初始化 Semaphore*/
    SemInit();

    /* 初始化 共享内存*/ 
    GetShmId(key);

    printf("End of initialize\n");
    return 0;
}
