#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAM 10000

int main()
{
    int segmento, segmento2, *t, *somaTot;
    
    segmento = shmget(IPC_PRIVATE, TAM * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    t = (int *) shmat(segmento, 0, 0);
    
    segmento2 = shmget(IPC_PRIVATE, 10 * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    somaTot = (int *) shmat(segmento2, 0, 0);
    
    for (int i = 0; i < TAM; i++) {
        t[i] = 5;
    }

    for (int i = 0; i <10; i++) {
        if (fork() == 0) {
            int total = 0;
            for (int j = 0; j < 1000; j++) {
                int pos = i * 1000 + j;
                t[pos] *= 2;
                total += t[pos];
            }
            somaTot[i] = total;
            shmdt(t);  
            exit(0);
        }
    }

    // Parent waits for all children to complete
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }
    int final = 0;
    for (int i = 0; i < 10; i++) {
        final += somaTot[i];
    }

    printf("Total: %d\n", final);

    shmdt(t);
    shmctl(segmento, IPC_RMID, NULL);
    shmdt(somaTot);
    shmctl(segmento2, IPC_RMID, NULL);

    return 0;
}
