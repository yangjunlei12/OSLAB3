//common.c
#include "common.h"


int GetShmId(key_t key) //IPC键
{
    int shmid;

    shmid = shmget(key,SHM_SIZE,IPC_CREAT|0666); //得到一个共享内存标识符或创建一个共享内存对象并返回共享内存标识符,SHM_SIZE共享内存大小
						 //0666是存取权限
    if(shmid < 0)
    {
        perror("Receiver: Shmget Error");
        exit(EXIT_FAILURE);
    }

    return shmid;
}

/*
* create mutex + semaphore
* 初始化值
*/
void SemInit()
{
     /*
     * Funtion Prototype: 
     *
     *  sem_t *sem_open(const char *name, int oflag,
     *                  mode_t mode, unsigned int value);
     *                 
     * name     : MUTEX_NAME    "mutex_shm"
     * oflag    : O_CREAT       Create and initialize it if not exist
     * mode_t   : file perssion -rw-r--r--
     * value    : 1
     */
     if((sem_open(MUTEX_NAME,O_CREAT,0644,1)) < 0) //创建并初始化有名信号量：name 信号灯的外部名字
						   //oflag 选择创建或打开一个现有的信号灯
						   //mode 权限位置value 
						   //信号灯初始值
     {
        perror("sem_open");
        exit(EXIT_FAILURE);
     }

     if((sem_open(FULL_NAME,O_CREAT,0644,0)) < 0){//成功就返回信号量的指针
        perror("sem_open");
        exit(EXIT_FAILURE);
     }
}


/*
* close and unlink semaphore that we crated
*/
void SemDestroy()
{
    sem_t * mutexPtr = sem_open(MUTEX_NAME,O_CREAT); //sem_t是信号量的结构名
    sem_t * fullPtr= sem_open(FULL_NAME,O_CREAT);

    /* Destroy mutex */
    sem_close(mutexPtr);                // 关闭已经命名的信号量
    sem_unlink(MUTEX_NAME);         // 从系统中删除有名信号量

    /* Destory full*/
    sem_close(fullPtr);
    sem_unlink(FULL_NAME);
}


void  P(sem_t *semPtr)
{
    sem_wait(semPtr);                   //使信号量的值-1
}

void  V(sem_t *semPtr)
{
    sem_post(semPtr);                   //使信号量的值+1
}
