#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct msgbuf {
    
    long mtype;
    char mtext[1000];
} msgbuf;

sem_t mutex;
sem_t full;
sem_t empty;

pthread_t send;
pthread_t receive;

int msgid;

void * sender();
void * receiver();

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    
    msgid = msgget(0, 0666);
    
    printf("ID=%d\n", msgid);
    pthread_create(&send, NULL, sender, NULL);
    pthread_create(&receive, NULL, receiver, NULL);
    
    pthread_join(send, NULL);
    pthread_join(receive, NULL);
    
    return 0;
}

void * sender() {
    char input[100];
    struct msgbuf msg;
    msg.mtype = 1;
    
    while(1) {
        sem_wait(&empty);
        sem_wait(&mutex);
        printf("Please input:\n");
        
        scanf("%s", input);
        if(strcmp(input, "exit") == 0) {
            memcpy(msg.mtext, "end", sizeof("end"));
            msgsnd(msgid, &msg, sizeof(msg), 0);
            printf("SENT: %s\n", msg.mtext);
            sem_post(&full);
            sem_post(&mutex);
            break;
        }
        
        memcpy(msg.mtext, input, sizeof(input));
        msgsnd(msgid, &msg, sizeof(msg), 0);
        printf("SENT: %s\n", msg.mtext);
        sem_post(&full);
        sem_post(&mutex);
    }
    
    memset(&msg, '\0', sizeof(msgbuf));
    msgrcv(msgid, &msg, sizeof(msgbuf), 2, 0);
    printf("END: %s\n", msg.mtext);
    
    exit(EXIT_SUCCESS);
}

void * receiver() {
    struct msgbuf msg;
    msg.mtype = 1;
    while(1) {
        sem_wait(&full);
        sem_wait(&mutex);
        
        msgrcv(msgid, &msg, sizeof(msgbuf), 1, 0);
        printf("RECEIVED: %s\n", msg.mtext);
        if(strcmp(msg.mtext, "end") == 0) {
            msg.mtype = 2;
            memcpy(msg.mtext, "over", sizeof("over"));
            msgsnd(msgid, &msg, sizeof(msgbuf), 0);
            
            sem_post(&mutex);
            break;
        }
        sem_post(&empty);
        sem_post(&mutex);
    }
    exit(EXIT_SUCCESS);
}

