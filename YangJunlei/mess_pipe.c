#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define Max 100

void check_err(int err) {
    if(err < 0) {
        fprintf(stderr, "Error : %s \n",strerror(errno) );
        exit(EXIT_FAILURE);
    }
}

int main() {
    sem_t mutex;
    sem_init(&mutex, 0, 1);
    int fd[2];
    pid_t pid[3];

    check_err(pipe(fd));

    pid[0] = fork();
    check_err(pid[0]);
    if(pid[0] == 0) {
        sem_wait(&mutex);
        close(fd[0]);

        char message[Max] = "Child process1's message\n";
        write(fd[1], message, sizeof(message));

        sem_post(&mutex);
    }

    pid[1] = fork();
    check_err(pid[1]);
    if(pid[1] == 0) {
        sem_wait(&mutex);
        close(fd[0]);

        char message[Max] = "Child process2's message\n";
        write(fd[1], message, sizeof(message));

        sem_post(&mutex);
    }

    pid[2] = fork();
    check_err(pid[2]);
    if(pid[2] == 0) {
        sem_wait(&mutex);
        close(fd[0]);

        char message[Max] = "Child process3's message\n";
        write(fd[1], message, sizeof(message));

        sem_post(&mutex);
    }

    char buf[Max];
    // 1
    close(fd[1]);
    read(fd[0], buf, sizeof(buf));
    printf("%s", buf);
    //2
    close(fd[1]);
    read(fd[0], buf, sizeof(buf));
    printf("%s", buf);
    //3
    close(fd[1]);
    read(fd[0], buf, sizeof(buf));
    printf("%s", buf);

    return 0;
}
