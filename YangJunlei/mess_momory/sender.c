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

    char input[1024];
    printf("Please input message!\n");
    scanf("%s", input);

    sem_wait(mutex);
    strcpy(shmp, input);
    sem_post(mutex);
    sem_post(full);

    return 0;
}
