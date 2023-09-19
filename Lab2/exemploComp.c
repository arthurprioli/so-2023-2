#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int segmento,pid, status;
	char* p;
	segmento = shmget(IPC_PRIVATE, 50*sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	p = (char *) shmat(segmento, 0, 0);
	if (fork() == 0){	
		printf("Mensagem processo filho 1: %s\n", p);
	}
	else if(fork() == 0){
		printf("Mensagem processo filho 2: %s\n", p);
	} else {
		printf("Processo pai...\n");
		strcpy(p, "oi bethlem");
	}
	shmdt(p);
	shmctl(segmento, IPC_RMID, 0);
	return 0;
}
