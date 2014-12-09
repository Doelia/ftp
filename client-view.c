#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/sem.h>

void displayFile() {
    FILE *f = fopen("logs.txt","rt");
    char c;
    while((c=fgetc(f))!=EOF){
        printf("%c",c);
    }
    fclose(f);
}

void waitSignal() {

    key_t key = ftok("./sem_disp", 10);
    if (key == -1) {
        perror("Problème lors de la réservation de la clé");
        exit(0);
    }

    int semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Problème lors du semget()");
        exit(0);
    }

    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

int main() {
    while (1) {
        system("clear");
        waitSignal();
        displayFile();
    }
    
}

