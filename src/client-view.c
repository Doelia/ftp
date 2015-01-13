#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/sem.h>

key_t key;
int semid ;
char fileLogs[21];

void displayFile() {
    FILE *f = fopen(fileLogs,"r");
    if (f == NULL) {
        perror("Erreur à l'ouverture du fichier.");
    }
    char c;
    while ((c=fgetc(f))!=EOF) {
        printf("%c", c);
    }
    printf("\n");
    fclose(f);
}

void waitSignal() {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

int main(int argx, char** charg) {

    char nameFile[21];
    sprintf(nameFile, "/tmp/sem_view%s", charg[1]);
    sprintf(fileLogs, "/tmp/logs_%s.txt", charg[1]);

    key = ftok(nameFile, 10);
    if (key == -1) {
        perror("Problème lors de la réservation de la clé");
        exit(0);
    }

    semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Problème lors du semget()");
        exit(0);
    }

    system("clear");
    while (1) {
        waitSignal();
        system("clear");
        displayFile();
    }
    
}

