#include "share_momery.h"

int main() {
    int shmid = shmget(KEY_NUM, 1024, 0666|IPC_CREAT);
    if(shmid < -1) {
        fprintf(stderr, "shmget Error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char * shmp = shmat(shmid, NULL, 0);

    full = sem_open("full_shm", O_CREAT, 066, 0);
    mutex = sem_open("mutex_shm", O_CREAT, 0666, 1);

    char result[1024];

    sem_wait(full);
    sem_wait(mutex);
    strcpy(result, shmp);
    sem_post(mutex);
    
    printf("received message: %s\n", result);

    sem_close(mutex);
    sem_unlink("mutex_shm");

    sem_close(full);
    sem_unlink("mutex_shm");

    return 0;
}
