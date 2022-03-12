#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5

int main() {

    int i, j, pid;

    // Edits
    int *turn;
    int shmid;
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    turn = (int*) shmat(shmid, NULL, 0);
    turn[0] = 0;
    // End of edits

    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {
            break;
        }
    }

    if(pid == 0) {
        // Edits 
        while(turn[0]!=i);
        // End of edits

        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");

        // Edits
        turn[0]++;
        // End

    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);
        shmdt(turn);
        shmctl(shmid, IPC_RMID, 0);
    }
}

