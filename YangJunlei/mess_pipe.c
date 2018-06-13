#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <wait.h>

#define Max 100

void check_err(int err) {
    if(err < 0) {
        fprintf(stderr, "Error : %s \n",strerror(errno) );
        exit(EXIT_FAILURE);
    }
}

int main() {
    sem_t mutex;
    sem_init(&mutex, 1, 1);
    int fd[2];
    pid_t pid[3] = {-1, -1, -1} ;
    pid_t current_pid = getpid();
    check_err(pipe(fd));

    pid[0] = fork();
    check_err(pid[0]);
    if(pid[0] == 0) {
        sem_wait(&mutex);
        close(fd[0]);

        char message[Max] = "Child process1's message\n";
        write(fd[1], message, sizeof(message));
        printf("child 1 sending message\n");
        sem_post(&mutex);
    }
    else {
        pid[1] = fork();
        check_err(pid[1]);
        if(pid[1] == 0) {
            sem_wait(&mutex);
            close(fd[0]);

            char message[Max] = "Child process2's message\n";
            write(fd[1], message, sizeof(message));
        printf("child 2 sending message\n");
            sem_post(&mutex);
        }
        else {
            pid[2] = fork();
            check_err(pid[2]);
            if(pid[2] == 0) {
                sem_wait(&mutex);
                close(fd[0]);

                char message[Max] = "Child process3's message\n";
                write(fd[1], message, sizeof(message));
            printf("child 3 sending message\n");
                sem_post(&mutex);
            }
            else {
            
                if(getpid() == current_pid &&
                pid[0] != -1 && pid[1] != -1 && pid[2] != -1){
                    char buf[Max];
                // 1
                wait(0);
                    close(fd[1]);
                    read(fd[0], buf, sizeof(buf));
                    printf("%s", buf);
                //2
                wait(0);
                    close(fd[1]);
                    read(fd[0], buf, sizeof(buf));
                    printf("%s", buf);
                //3
                wait(0);
                    close(fd[1]);
                    read(fd[0], buf, sizeof(buf));
                    printf("%s", buf);
                }
            }
        }
    }
    return 0;
}
