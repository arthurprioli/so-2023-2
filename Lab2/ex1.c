#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAM 10000

int main()
{
    int segmento, *t;
    segmento = shmget(IPC_PRIVATE, TAM * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    t = (int *) shmat(segmento, 0, 0);
    for (int i = 0; i < TAM; i++){
        t[i] = 5;
    }
    for (int i = 0; i <10; i++){
        if (fork() == 0){
            int somaTot = 0;
            for (int j = 0; j < 1000; j++){
                int pos = i*1000 + j;
                t[pos] *= 2;
                somaTot += t[pos]; 
            }
            exit(0);
        } 
    }
    return 0;
}