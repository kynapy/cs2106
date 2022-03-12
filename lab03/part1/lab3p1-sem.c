#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5
 
int main() {

    int i, j, pid;
    int shmid[NUM_PROCESSES];
    sem_t * sems[NUM_PROCESSES];
    for (int x = 0; x < NUM_PROCESSES; x++) {
        shmid[x] = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
        sems[x] = (sem_t *) shmat(shmid[x], NULL, 0);
        if (x == 0) {
            sem_init(sems[x],1,1);
        } else {
            sem_init(sems[x],1,0);
        }
    }

    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {
            break;
        }
    }

    if(pid == 0) {
        sem_wait(sems[i]);
        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");
        sem_post(sems[i+1]);
    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);
        for(int x = 0; x < NUM_PROCESSES; x++) {
            sem_destroy(sems[x]);
            shmdt(sems[x]);
            shmctl(shmid[x], IPC_RMID, 0);
        }
    }

}

