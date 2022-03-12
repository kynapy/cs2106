#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5

int main() {

    int i, j, pid;
    int shmid1, shmid2, shmid3, shmid4;
    sem_t *sem1, *sem2, *sem3, *sem4;

    shmid1 = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    shmid2 = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    shmid3 = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    shmid4 = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);


    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {
            break;
        }
    }

    if(pid == 0) {
        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");
        //sem_post(sem[i+1]);
    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);
            //sem_destroy(sem1);
            //sem_destroy(sem2);
            //sem_destroy(sem3);
            //sem_destroy(sem4);
            //shmctl(shmid, IPC_RMID, 0);
    }

}

