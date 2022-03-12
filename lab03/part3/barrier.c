#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>

int nproc, barrier_shmid, countBlockShmid, countShmid;
sem_t *barrier, *count_block;
int *count;

void init_barrier(int numproc) {
    nproc = numproc;
    
    // for barrier semaphore
    barrier_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    barrier = (sem_t *) shmat(barrier_shmid, NULL, 0);
    sem_init(barrier, 1, 0);
    
    // for count blocker semaphore
    countBlockShmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    count_block = (sem_t *) shmat(countBlockShmid, NULL, 0);
    sem_init(count_block, 1, 1);

    // for shared variable count
    countShmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    count = (int*) shmat(countShmid, NULL, 0);
    count[0] = 0;
}

void reach_barrier() {
    sem_wait(count_block);
    count[0]+=1;
    sem_post(count_block);
    if (count[0] == nproc) {
        sem_post(barrier);
    } else {
        sem_wait(barrier);
        sem_post(barrier);
    }
}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(barrier);
        sem_destroy(count_block);
        shmdt(barrier);
        shmdt(count_block);
        shmdt(count);
        shmctl(barrier_shmid, IPC_RMID, 0);
        shmctl(countBlockShmid, IPC_RMID, 0);
        shmctl(countShmid, IPC_RMID, 0);
    }
}


